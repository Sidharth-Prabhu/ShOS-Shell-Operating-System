// klib.h
#ifndef KLIB_H
#define KLIB_H

#include "vga.h"

// Declare I/O functions before use
static inline unsigned char inb(unsigned short port);
static inline void outb(unsigned short port, unsigned char val);

// Declare functions
char kgetchar(void);
void kputs(const char *s);

int kstrlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

int kstreq(const char *s1, const char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 == *s2;
}

void kgets(char *buf, int max) {
    int i = 0;
    while (i < max - 1) {
        char c = kgetchar();
        if (c == '\n') break;
        if (c == '\b' && i > 0) {
            i--;
            vga_putc('\b');
            vga_putc(' ');
            vga_putc('\b');
        } else if (c >= 32 && c <= 126) {
            buf[i++] = c;
            vga_putc(c);
        }
    }
    buf[i] = '\0';
    vga_putc('\n');
}

// Improved keyboard input handling
char kgetchar(void) {
    while ((inb(0x64) & 1) == 0); // Wait for data
    unsigned char scancode = inb(0x60);
    
    // Basic scancode to ASCII conversion (US QWERTY layout)
    static const char keymap[128] = {
        0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   // 0-14
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',       // 15-28
        0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\',  // 29-43
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,   '*',  0,   ' ', 0,     // 44-58
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,     // 59-73
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,     // 74-88
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,     // 89-103
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,     // 104-118
        0,   0,   0,   0,   0,   0,   0,   0,   0                                       // 119-127
    };
    
    if (scancode < 0x80) { // Key press
        return keymap[scancode];
    }
    return 0;
}

// I/O functions
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

#endif