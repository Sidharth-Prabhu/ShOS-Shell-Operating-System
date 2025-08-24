// kernel.c
#include "vga.h"
#include "shell.h"
#include "splash.h"
#include "auth.h"
#include "login.h"
#include "kernel/klib.h"
#include "fs/fs.h"
#include "drivers/vga.h"

void kmain() {
    show_splash_screen();
    
    // Initialize systems - filesystem FIRST
    fs_init();
    auth_init(); // This depends on filesystem
    
    // Show login menu until successful login
    int login_successful = 0;
    while (!login_successful) {
        login_show_menu();
        login_successful = login_handle_choice();
    }
    
    // Login successful, start shell
    vga_clear();
    vga_puts("Welcome to ShOS!\n");
    shell_run();
}