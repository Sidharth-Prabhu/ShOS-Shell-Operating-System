// editor.c
#include "vga.h"
#include "klib.h"
#include "fs.h"

#define EDITOR_BUFFER_SIZE 1024

void editor_nano(const char *filename) {
    char buffer[EDITOR_BUFFER_SIZE] = {0};
    int cursor = 0;
    int editing = 1;
    
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
        char c = kgetchar();
        
        if (c == 0x01) { // Ctrl+A - beginning of line
            cursor = 0;
            // Would need cursor positioning in real editor
        }
        else if (c == 0x05) { // Ctrl+E - end of line
            cursor = kstrlen(buffer);
            // Would need cursor positioning in real editor
        }
        else if (c == 0x13) { // Ctrl+S - save
            fs_write(filename, buffer);
            vga_puts("\nFile saved successfully!\n");
            editing = 0;
        }
        else if (c == 0x18) { // Ctrl+X - exit without saving
            vga_puts("\nExiting without saving.\n");
            editing = 0;
        }
        else if (c == '\b') { // Backspace
            if (cursor > 0) {
                // Shift everything left
                for (int i = cursor - 1; i < kstrlen(buffer); i++) {
                    buffer[i] = buffer[i + 1];
                }
                cursor--;
                vga_putc('\b');
                vga_putc(' ');
                vga_putc('\b');
            }
        }
        else if (c == '\n') { // Enter
            if (cursor < EDITOR_BUFFER_SIZE - 1) {
                buffer[cursor++] = '\n';
                vga_putc('\n');
            }
        }
        else if (c >= 32 && c <= 126) { // Printable characters
            if (cursor < EDITOR_BUFFER_SIZE - 1) {
                // Make space for new character
                for (int i = kstrlen(buffer); i >= cursor; i--) {
                    buffer[i + 1] = buffer[i];
                }
                buffer[cursor++] = c;
                vga_putc(c);
            }
        }
    }
    
    // Return to shell
    vga_puts("\nPress any key to return to shell...\n");
    kgetchar();
    vga_clear();
}