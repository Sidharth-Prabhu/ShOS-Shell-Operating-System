// klib.h
#ifndef KLIB_H
#define KLIB_H

#include "vga.h"

// Declare I/O functions
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char val);

// Declare functions
char kgetchar(void);
int kstrlen(const char *s);
int kstreq(const char *s1, const char *s2);
void kgets(char *buf, int max);

// Utility functions
int str_to_int(const char *str);
void int_to_str(int num, char *str);
void kstrcpy(char *dest, const char *src);
void kstrcat(char *dest, const char *src);

#endif