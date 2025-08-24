// login.c
#include "login.h"
#include "auth.h"
#include "vga.h"
#include "klib.h"

void login_show_menu(void) {
    vga_clear();
    vga_puts("ShOS Login \n");
    vga_puts("========== \n");
    vga_puts("1. Login \n");
    vga_puts("2. Register \n");
    vga_puts("3. Shutdown system \n");
    vga_puts("Enter any option (1 - 3): ");
}

int login_handle_choice(void) {
    char choice[4] = {0};
    kgets(choice, 4);
    
    if (kstreq(choice, "1")) {
        return auth_login(); // Returns 1 if login successful
    } else if (kstreq(choice, "2")) {
        if (auth_register()) {
            // Registration successful, return to main menu
            return 0; // Return 0 to indicate we should show menu again
        } else {
            // Registration failed, show menu again
            return 0;
        }
    } else if (kstreq(choice, "3")) {
        vga_puts("Shutting down...\n");
        asm volatile ("cli");
        asm volatile ("hlt");
        return 0;
    } else {
        vga_puts("Invalid choice. Please try again.\n");
        for (volatile int i = 0; i < 1000000; i++);
        return 0; // Show menu again
    }
}