CC = gcc
AS = nasm
LD = ld

CFLAGS = -ffreestanding -m32 -Wall -Wextra
LDFLAGS = -T linker.ld -melf_i386
ASFLAGS = -f elf32

all: myos.bin

myos.bin: boot.o kernel.o shell.o vga.o klib.o tictactoe.o splash.o fs.o editor.o
	$(LD) $(LDFLAGS) -o $@ $^

editor.o: editor.c editor.h vga.h klib.h fs.h
	$(CC) $(CFLAGS) -c -o $@ $<

fs.o: fs.c fs.h vga.h klib.h
	$(CC) $(CFLAGS) -c -o $@ $<

boot.o: boot.asm
	$(AS) $(ASFLAGS) -o $@ $<

kernel.o: kernel.c vga.h shell.h splash.h
	$(CC) $(CFLAGS) -c -o $@ $<

shell.o: shell.c vga.h klib.h shell.h tictactoe.h
	$(CC) $(CFLAGS) -c -o $@ $<

vga.o: vga.c vga.h
	$(CC) $(CFLAGS) -c -o $@ $<

klib.o: klib.c klib.h vga.h
	$(CC) $(CFLAGS) -c -o $@ $<

tictactoe.o: tictactoe.c tictactoe.h vga.h klib.h
	$(CC) $(CFLAGS) -c -o $@ $<

splash.o: splash.c splash.h vga.h klib.h
	$(CC) $(CFLAGS) -c -o $@ $<

run: myos.bin
	qemu-system-i386 -kernel $<

clean:
	rm -f *.o myos.bin