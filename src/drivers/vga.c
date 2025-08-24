// vga.c
#include "vga.h"

unsigned short *vga_buffer = (unsigned short *)VGA_BUFFER;
int vga_x = 0, vga_y = 0;

void vga_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga_buffer[i] = (0x07 << 8) | ' '; // White on black
    vga_x = 0;
    vga_y = 0;
}

void vga_putc(char c) {
    if (c == '\n') {
        vga_x = 0;
        vga_y++;
    } else if (c == '\b') {
        if (vga_x > 0) {
            vga_x--;
        } else if (vga_y > 0) {
            vga_y--;
            vga_x = VGA_WIDTH - 1;
        }
        vga_buffer[vga_y * VGA_WIDTH + vga_x] = (0x07 << 8) | ' ';
    } else {
        vga_buffer[vga_y * VGA_WIDTH + vga_x] = (0x07 << 8) | c;
        vga_x++;
    }
    
    if (vga_x >= VGA_WIDTH) {
        vga_x = 0;
        vga_y++;
    }
    
    if (vga_y >= VGA_HEIGHT) {
        // Scroll the screen
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        // Clear the last line
        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            vga_buffer[i] = (0x07 << 8) | ' ';
        }
        vga_y = VGA_HEIGHT - 1;
    }
}

void vga_putc_at(int x, int y, char c) {
    if (x >= 0 && x < VGA_WIDTH && y >= 0 && y < VGA_HEIGHT) {
        vga_buffer[y * VGA_WIDTH + x] = (0x07 << 8) | c;
    }
}

void vga_puts(const char *s) {
    while (*s) vga_putc(*s++);
}