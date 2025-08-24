// splash.c
#include "vga.h"
#include "klib.h"

void show_splash_screen(void) {
    vga_clear();
    
    // Draw a fancy border
    for (int x = 0; x < VGA_WIDTH; x++) {
        vga_putc_at(x, 0, '=');
        vga_putc_at(x, VGA_HEIGHT - 1, '=');
    }
    for (int y = 1; y < VGA_HEIGHT - 1; y++) {
        vga_putc_at(0, y, '|');
        vga_putc_at(VGA_WIDTH - 1, y, '|');
    }
    
    // Draw corners
    vga_putc_at(0, 0, '+');
    vga_putc_at(VGA_WIDTH - 1, 0, '+');
    vga_putc_at(0, VGA_HEIGHT - 1, '+');
    vga_putc_at(VGA_WIDTH - 1, VGA_HEIGHT - 1, '+');
    
    // Draw the OS name in big letters - ShOS
    int name_x = (VGA_WIDTH - 16) / 2; // Center the name
    
    // Draw S
    vga_putc_at(name_x + 0, 5, '#');
    vga_putc_at(name_x + 0, 6, '#');
    vga_putc_at(name_x + 0, 7, '#');
    vga_putc_at(name_x + 1, 5, '#');
    vga_putc_at(name_x + 1, 7, '#');
    vga_putc_at(name_x + 2, 5, '#');
    vga_putc_at(name_x + 2, 7, '#');
    vga_putc_at(name_x + 3, 6, '#');
    vga_putc_at(name_x + 3, 7, '#');
    
    // Draw h
    vga_putc_at(name_x + 5, 5, '#');
    vga_putc_at(name_x + 5, 6, '#');
    vga_putc_at(name_x + 5, 7, '#');
    vga_putc_at(name_x + 5, 8, '#');
    vga_putc_at(name_x + 6, 6, '#');
    vga_putc_at(name_x + 7, 6, '#');
    vga_putc_at(name_x + 8, 5, '#');
    vga_putc_at(name_x + 8, 6, '#');
    vga_putc_at(name_x + 8, 7, '#');
    vga_putc_at(name_x + 8, 8, '#');
    
    // Draw O
    vga_putc_at(name_x + 10, 5, '#');
    vga_putc_at(name_x + 10, 6, '#');
    vga_putc_at(name_x + 10, 7, '#');
    vga_putc_at(name_x + 11, 4, '#');
    vga_putc_at(name_x + 11, 8, '#');
    vga_putc_at(name_x + 12, 4, '#');
    vga_putc_at(name_x + 12, 8, '#');
    vga_putc_at(name_x + 13, 5, '#');
    vga_putc_at(name_x + 13, 6, '#');
    vga_putc_at(name_x + 13, 7, '#');
    
    // Draw S
    vga_putc_at(name_x + 15, 4, '#');
    vga_putc_at(name_x + 15, 5, '#');
    vga_putc_at(name_x + 15, 7, '#');
    vga_putc_at(name_x + 15, 8, '#');
    vga_putc_at(name_x + 16, 4, '#');
    vga_putc_at(name_x + 16, 6, '#');
    vga_putc_at(name_x + 16, 8, '#');
    vga_putc_at(name_x + 17, 4, '#');
    vga_putc_at(name_x + 17, 6, '#');
    vga_putc_at(name_x + 17, 8, '#');
    vga_putc_at(name_x + 18, 5, '#');
    vga_putc_at(name_x + 18, 6, '#');
    vga_putc_at(name_x + 18, 7, '#');
    
    // Draw version info
    char *version = "Version 1.0";
    int version_x = (VGA_WIDTH - kstrlen(version)) / 2;
    for (int i = 0; version[i]; i++) {
        vga_putc_at(version_x + i, 10, version[i]);
    }
    
    // Draw copyright
    char *copyright = "(C) 2024 ShOS Development Team";
    int copyright_x = (VGA_WIDTH - kstrlen(copyright)) / 2;
    for (int i = 0; copyright[i]; i++) {
        vga_putc_at(copyright_x + i, 12, copyright[i]);
    }
    
    // Draw loading message with animation
    char *loading = "Loading operating system";
    int loading_x = (VGA_WIDTH - kstrlen(loading)) / 2;
    
    for (int i = 0; loading[i]; i++) {
        vga_putc_at(loading_x + i, 15, loading[i]);
        // Delay for text animation (about 0.5 seconds total)
        for (volatile int j = 0; j < 800000; j++);
    }
    
    // Draw progress bar
    int bar_start = VGA_WIDTH / 4;
    int bar_end = 3 * VGA_WIDTH / 4;
    int bar_length = bar_end - bar_start;
    
    vga_putc_at(bar_start - 1, 17, '[');
    vga_putc_at(bar_end + 1, 17, ']');
    
    // Animated progress bar (about 2 seconds total)
    for (int progress = 0; progress <= bar_length; progress++) {
        for (int i = 0; i < progress; i++) {
            vga_putc_at(bar_start + i, 17, '=');
        }
        if (progress < bar_length) {
            vga_putc_at(bar_start + progress, 17, '>');
        }
        // Delay for progress bar animation
        for (volatile int j = 0; j < 1500000; j++);
    }
    
    // Final message
    char *ready = "System ready!";
    int ready_x = (VGA_WIDTH - kstrlen(ready)) / 2;
    for (int i = 0; ready[i]; i++) {
        vga_putc_at(ready_x + i, 19, ready[i]);
    }
    
    // Wait for exactly 5 seconds total (2.5 seconds after animations)
    for (volatile int i = 0; i < 12500000; i++);
    
    // Clear screen for the actual OS
    vga_clear();
}