// klib.c
#include "klib.h"
#include "vga.h"

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

void kstrcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

// String concatenate function
void kstrcat(char *dest, const char *src) {
    while (*dest) dest++;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
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

char kgetchar(void) {
    while ((inb(0x64) & 1) == 0); // Wait for data
    unsigned char scancode = inb(0x60);
    
    static const char keymap[128] = {
        0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,   '*',  0,   ' ', 0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0
    };
    
    if (scancode < 0x80) {
        return keymap[scancode];
    }
    return 0;
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

int str_to_int(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;
    
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }
    
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
            i++;
        } else {
            return 0;
        }
    }
    
    return result * sign;
}

void int_to_str(int num, char *str) {
    int i = 0;
    int is_negative = 0;
    
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    
    while (num != 0) {
        int digit = num % 10;
        str[i++] = '0' + digit;
        num = num / 10;
    }
    
    if (is_negative) {
        str[i++] = '-';
    }
    
    str[i] = '\0';
    
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}