// kernel.c
#include "vga.h"
#include "shell.h"
#include "splash.h"
#include "fs.h"

void kmain() {
    show_splash_screen();  // Show splash screen first
    vga_puts("Welcome to ShOS!\n");  // Changed from MyOS to ShOS
    fs_init();
    shell_run(); // Start shell
}