// vga.h
#ifndef VGA_H
#define VGA_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER 0xB8000

extern unsigned short *vga_buffer;
extern int vga_x, vga_y;

void vga_clear(void);
void vga_putc(char c);
void vga_puts(const char *s);
void vga_putc_at(int x, int y, char c);

#endif