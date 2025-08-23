CC = gcc
AS = nasm
LD = ld

CFLAGS = -ffreestanding -m32 -Wall -Wextra
LDFLAGS = -T linker.ld -melf_i386
ASFLAGS = -f elf32

all: myos.bin

myos.bin: boot.o kernel.o shell.o vga.o klib.o tictactoe.o
	$(LD) $(LDFLAGS) -o $@ $^

boot.o: boot.asm
	$(AS) $(ASFLAGS) -o $@ $<

kernel.o: kernel.c vga.h shell.h
	$(CC) $(CFLAGS) -c -o $@ $<

shell.o: shell.c vga.h klib.h shell.h pong.h
	$(CC) $(CFLAGS) -c -o $@ $<

vga.o: vga.c vga.h
	$(CC) $(CFLAGS) -c -o $@ $<

klib.o: klib.c klib.h vga.h
	$(CC) $(CFLAGS) -c -o $@ $<

tictactoe.o: tictactoe.c tictactoe.h vga.h klib.h  # Add this target
	$(CC) $(CFLAGS) -c -o $@ $<

run: myos.bin
	qemu-system-i386 -kernel $<

clean:
	rm -f *.o myos.bin