// shell.c
#include "vga.h"
#include "klib.h"
#include "shell.h"

#define MAX_INPUT 80
#define MAX_ARGS 10

// Function prototypes for commands
void cmd_help(char *args[]);
void cmd_clear(char *args[]);
void cmd_echo(char *args[]);
void cmd_info(char *args[]);
void cmd_shutdown(char *args[]);

// Command structure
typedef struct {
    const char *name;
    void (*func)(char *args[]);
    const char *description;
} Command;

// Available commands
Command commands[] = {
    {"help", cmd_help, "Show available commands"},
    {"clear", cmd_clear, "Clear the screen"},
    {"echo", cmd_echo, "Echo arguments back to the screen"},
    {"info", cmd_info, "Show system information"},
    {"shutdown", cmd_shutdown, "Shutdown the system"},
    {0, 0, 0} // End marker
};

// Parse input into arguments
int parse_args(char *input, char *args[]) {
    int argc = 0;
    char *p = input;
    
    while (*p && argc < MAX_ARGS - 1) {
        // Skip leading spaces
        while (*p == ' ') p++;
        if (!*p) break;
        
        // Mark start of argument
        args[argc++] = p;
        
        // Find end of argument
        while (*p && *p != ' ') p++;
        if (*p) *p++ = '\0'; // Null-terminate and move to next
    }
    
    args[argc] = 0; // Null-terminate argument list
    return argc;
}

// Command implementations
void cmd_help(char *args[]) {
    (void)args; // Unused parameter
    vga_puts("Available commands:\n");
    for (int i = 0; commands[i].name; i++) {
        vga_puts("  ");
        vga_puts(commands[i].name);
        vga_puts(" - ");
        vga_puts(commands[i].description);
        vga_puts("\n");
    }
}

void cmd_clear(char *args[]) {
    (void)args; // Unused parameter
    vga_clear();
}

void cmd_echo(char *args[]) {
    for (int i = 1; args[i]; i++) {
        vga_puts(args[i]);
        if (args[i + 1]) vga_puts(" ");
    }
    vga_puts("\n");
}

void cmd_info(char *args[]) {
    (void)args; // Unused parameter
    vga_puts("MyOS v0.1\n");
    vga_puts("Simple operating system kernel\n");
    vga_puts("VGA text mode: 80x25\n");
}

void cmd_shutdown(char *args[]) {
    (void)args; // Unused parameter
    vga_puts("System shutting down...\n");
    // In a real OS, we would actually shut down the system
    // For now, just halt
    asm volatile ("cli");
    asm volatile ("hlt");
}

// Find and execute command
void execute_command(char *input) {
    char *args[MAX_ARGS];
    int argc = parse_args(input, args);
    
    if (argc == 0) return;
    
    for (int i = 0; commands[i].name; i++) {
        if (kstreq(args[0], commands[i].name)) {
            commands[i].func(args);
            return;
        }
    }
    
    vga_puts("Unknown command: ");
    vga_puts(args[0]);
    vga_puts("\nType 'help' for available commands.\n");
}

void shell_run() {
    char input[MAX_INPUT];
    
    vga_puts("MyOS Shell - Type 'help' for available commands\n");
    
    while (1) {
        vga_puts("> ");
        kgets(input, MAX_INPUT);
        execute_command(input);
    }
}