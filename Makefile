# Directory paths
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
BOOT_SRC = $(SRC_DIR)/boot/boot.asm
KERNEL_SRC = $(SRC_DIR)/kernel/kernel.c
KLIB_SRC = $(SRC_DIR)/kernel/klib.c
FS_SRC = $(SRC_DIR)/fs/fs.c
VGA_SRC = $(SRC_DIR)/drivers/vga.c
AUTH_SRC = $(SRC_DIR)/auth/auth.c
LOGIN_SRC = $(SRC_DIR)/auth/login.c
SHELL_SRC = $(SRC_DIR)/apps/shell.c
EDITOR_SRC = $(SRC_DIR)/apps/editor.c
TICTACTOE_SRC = $(SRC_DIR)/apps/tictactoe.c
SPLASH_SRC = $(SRC_DIR)/ui/splash.c

# Object files
BOOT_OBJ = $(BUILD_DIR)/boot.o
KERNEL_OBJ = $(BUILD_DIR)/kernel.o
KLIB_OBJ = $(BUILD_DIR)/klib.o
FS_OBJ = $(BUILD_DIR)/fs.o
VGA_OBJ = $(BUILD_DIR)/vga.o
AUTH_OBJ = $(BUILD_DIR)/auth.o
LOGIN_OBJ = $(BUILD_DIR)/login.o
SHELL_OBJ = $(BUILD_DIR)/shell.o
EDITOR_OBJ = $(BUILD_DIR)/editor.o
TICTACTOE_OBJ = $(BUILD_DIR)/tictactoe.o
SPLASH_OBJ = $(BUILD_DIR)/splash.o

# Linker script
LINKER_SCRIPT = linker.ld

# Include paths
INCLUDES = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/kernel -I$(INCLUDE_DIR)/fs \
           -I$(INCLUDE_DIR)/drivers -I$(INCLUDE_DIR)/auth \
           -I$(INCLUDE_DIR)/apps -I$(INCLUDE_DIR)/ui

# Compiler and flags (32-bit version)
CC = gcc
ASM = nasm
LD = ld
CFLAGS = -ffreestanding -nostdlib -Wall -Wextra -m32
ASMFLAGS = -f elf32
LDFLAGS = -T $(LINKER_SCRIPT) -nostdlib -m elf_i386

# Default target
all: $(BUILD_DIR)/myos.bin

# Bootloader
$(BOOT_OBJ): $(BOOT_SRC)
	$(ASM) $(ASMFLAGS) $< -o $@

# Kernel
$(KERNEL_OBJ): $(KERNEL_SRC)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Kernel library
$(KLIB_OBJ): $(KLIB_SRC)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# File system
$(FS_OBJ): $(FS_SRC)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# VGA driver
$(VGA_OBJ): $(VGA_SRC)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Authentication
$(AUTH_OBJ): $(AUTH_SRC)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Login
$(LOGIN_OBJ): $(LOGIN_SRC)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Shell
$(SHELL_OBJ): $(SHELL_SRC)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Editor
$(EDITOR_OBJ): $(EDITOR_SRC)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Tic Tac Toe
$(TICTACTOE_OBJ): $(TICTACTOE_SRC)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Splash screen
$(SPLASH_OBJ): $(SPLASH_SRC)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Final binary
$(BUILD_DIR)/myos.bin: $(BOOT_OBJ) $(KERNEL_OBJ) $(KLIB_OBJ) $(FS_OBJ) $(VGA_OBJ) \
          $(AUTH_OBJ) $(LOGIN_OBJ) $(SHELL_OBJ) $(EDITOR_OBJ) \
          $(TICTACTOE_OBJ) $(SPLASH_OBJ) $(LINKER_SCRIPT)
	$(LD) $(LDFLAGS) -o $@ $(filter-out $(LINKER_SCRIPT),$^)

# Check if linker script exists
$(LINKER_SCRIPT):
	@echo "Creating basic linker script for 32-bit kernel..."
	@echo 'ENTRY(_start)' > $@
	@echo 'OUTPUT_FORMAT(binary)' >> $@
	@echo '' >> $@
	@echo 'SECTIONS' >> $@
	@echo '{' >> $@
	@echo '    . = 0x100000; /* 1MB mark */' >> $@
	@echo '    .text : ALIGN(4096) { *(.text) }' >> $@
	@echo '    .rodata : ALIGN(4096) { *(.rodata) }' >> $@
	@echo '    .data : ALIGN(4096) { *(.data) }' >> $@
	@echo '    .bss : ALIGN(4096) { *(COMMON) *(.bss) }' >> $@
	@echo '}' >> $@

# Run the OS in QEMU
run: $(BUILD_DIR)/myos.bin
	qemu-system-x86_64 -kernel $(BUILD_DIR)/myos.bin

# Run with debug output
debug: $(BUILD_DIR)/myos.bin
	qemu-system-x86_64 -kernel $(BUILD_DIR)/myos.bin -d cpu_reset -no-reboot -no-shutdown

# Run with serial output
serial: $(BUILD_DIR)/myos.bin
	qemu-system-x86_64 -kernel $(BUILD_DIR)/myos.bin -serial stdio

# Create a bootable ISO image
iso: $(BUILD_DIR)/myos.bin
	mkdir -p $(BUILD_DIR)/isodir/boot/grub
	cp $(BUILD_DIR)/myos.bin $(BUILD_DIR)/isodir/boot/
	echo 'menuentry "SHOS" {' > $(BUILD_DIR)/isodir/boot/grub/grub.cfg
	echo '    multiboot /boot/myos.bin' >> $(BUILD_DIR)/isodir/boot/grub/grub.cfg
	echo '    boot' >> $(BUILD_DIR)/isodir/boot/grub/grub.cfg
	echo '}' >> $(BUILD_DIR)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/shos.iso $(BUILD_DIR)/isodir

# Run from ISO
run-iso: iso
	qemu-system-x86_64 -cdrom $(BUILD_DIR)/shos.iso

# Clean build files
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/myos.bin

# Clean everything including ISO
distclean: clean
	rm -rf $(BUILD_DIR)/isodir $(BUILD_DIR)/shos.iso

.PHONY: all clean run debug serial iso run-iso distclean