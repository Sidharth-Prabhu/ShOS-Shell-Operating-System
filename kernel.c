// kernel.c
#include "vga.h"
#include "shell.h"

void kmain() {
    vga_clear(); // Clear screen
    vga_puts("Welcome to MyOS!\n");
    shell_run(); // Start shell
}