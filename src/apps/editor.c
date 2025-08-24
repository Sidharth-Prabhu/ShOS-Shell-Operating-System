// editor.c
#include "vga.h"
#include "klib.h"
#include "fs.h"

#define EDITOR_BUFFER_SIZE 1024

// Keyboard scancodes for special keys
#define KEY_CTRL 0x1D
#define KEY_CTRL_RELEASE 0x9D
#define KEY_S 0x1F
#define KEY_X 0x2D

void editor_nano(const char *filename) {
    char buffer[EDITOR_BUFFER_SIZE] = {0};
    int cursor = 0;
    int editing = 1;
    int ctrl_pressed = 0;
    
    vga_clear();
    vga_puts("ShOS Text Editor - Editing: ");
    vga_puts(filename);
    vga_puts("\n");
    vga_puts("Press Ctrl+S to save, Ctrl+X to exit without saving\n");
    vga_puts("---------------------------------------------------\n");
    
    // Load existing file content if it exists
    fs_node *file = fs_find_file(filename);
    if (file != NULL && file->data != NULL) {
        kstrcpy(buffer, file->data);
        cursor = kstrlen(buffer);
        vga_puts(buffer);
    }
    
    while (editing) {
        // Check if key is available
        if ((inb(0x64) & 1) == 0) {
            continue; // No key available
        }
        
        unsigned char scancode = inb(0x60);
        
        // Handle key press (scancode < 0x80)
        if (scancode < 0x80) {
            // Check for Ctrl key
            if (scancode == KEY_CTRL) {
                ctrl_pressed = 1;
                continue;
            }
            
            // If Ctrl is pressed, check for combinations
            if (ctrl_pressed) {
                if (scancode == KEY_S) { // Ctrl+S - save
                    fs_write(filename, buffer);
                    vga_puts("\nFile saved successfully!\n");
                    editing = 0;
                    continue;
                }
                else if (scancode == KEY_X) { // Ctrl+X - exit without saving
                    vga_puts("\nExiting without saving.\n");
                    editing = 0;
                    continue;
                }
            }
            
            // Handle normal key input
            if (scancode == 0x0E) { // Backspace
                if (cursor > 0) {
                    cursor--;
                    buffer[cursor] = '\0';
                    vga_putc('\b');
                    vga_putc(' ');
                    vga_putc('\b');
                }
            }
            else if (scancode == 0x1C) { // Enter
                if (cursor < EDITOR_BUFFER_SIZE - 1) {
                    buffer[cursor++] = '\n';
                    vga_putc('\n');
                }
            }
            else {
                // Convert scancode to ASCII (simplified)
                char c = 0;
                switch (scancode) {
                    case 0x10: c = 'q'; break;
                    case 0x11: c = 'w'; break;
                    case 0x12: c = 'e'; break;
                    case 0x13: c = 'r'; break;
                    case 0x14: c = 't'; break;
                    case 0x15: c = 'y'; break;
                    case 0x16: c = 'u'; break;
                    case 0x17: c = 'i'; break;
                    case 0x18: c = 'o'; break;
                    case 0x19: c = 'p'; break;
                    case 0x1E: c = 'a'; break;
                    case 0x1F: c = 's'; break;
                    case 0x20: c = 'd'; break;
                    case 0x21: c = 'f'; break;
                    case 0x22: c = 'g'; break;
                    case 0x23: c = 'h'; break;
                    case 0x24: c = 'j'; break;
                    case 0x25: c = 'k'; break;
                    case 0x26: c = 'l'; break;
                    case 0x2C: c = 'z'; break;
                    case 0x2D: c = 'x'; break;
                    case 0x2E: c = 'c'; break;
                    case 0x2F: c = 'v'; break;
                    case 0x30: c = 'b'; break;
                    case 0x31: c = 'n'; break;
                    case 0x32: c = 'm'; break;
                    case 0x39: c = ' '; break;
                    // Add more key mappings as needed
                }
                
                if (c != 0 && cursor < EDITOR_BUFFER_SIZE - 1) {
                    buffer[cursor++] = c;
                    vga_putc(c);
                }
            }
        }
        else {
            // Handle key release (scancode >= 0x80)
            unsigned char release_code = scancode - 0x80;
            if (release_code == KEY_CTRL) {
                ctrl_pressed = 0;
            }
        }
    }
    
    // Return to shell
    vga_puts("\nPress any key to return to shell...\n");
    
    // Wait for any key
    while ((inb(0x64) & 1) == 0); // Wait for key
    inb(0x60); // Read and discard the key
    
    vga_clear();
}