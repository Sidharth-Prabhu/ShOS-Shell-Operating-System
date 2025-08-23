// shell.c
#ifndef NULL
#define NULL ((void*)0)
#endif
#include "vga.h"
#include "klib.h"
#include "shell.h"
#include "tictactoe.h"
#include "fs.h"
#include "editor.h"
#include <stddef.h>

#define MAX_INPUT 80
#define MAX_ARGS 10

// Function prototypes for commands
void cmd_help(char *args[]);
void cmd_clear(char *args[]);
void cmd_echo(char *args[]);
void cmd_info(char *args[]);
void cmd_shutdown(char *args[]);
void cmd_add(char *args[]);
void cmd_subtract(char *args[]);
void cmd_multiply(char *args[]);
void cmd_divide(char *args[]);
void cmd_tictactoe(char *args[]);
void cmd_mkdir(char *args[]);
void cmd_ls(char *args[]);
void cmd_pwd(char *args[]);
void cmd_cd(char *args[]);
void cmd_touch(char *args[]);
void cmd_cat(char *args[]);
void cmd_edit(char *args[]);
void cmd_write(char *args[]);
void cmd_rm(char *args[]);

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
    {"add", cmd_add, "Add two numbers: add <num1> <num2>"},
    {"sub", cmd_subtract, "Subtract two numbers: sub <num1> <num2>"},
    {"mul", cmd_multiply, "Multiply two numbers: mul <num1> <num2>"},
    {"div", cmd_divide, "Divide two numbers: div <num1> <num2>"},
    {"tictactoe", cmd_tictactoe, "Play Tic Tac Toe game"},
    {"mkdir", cmd_mkdir, "Create directory: mkdir <dirname>"},
    {"ls", cmd_ls, "List directory contents: ls [path]"},
    {"pwd", cmd_pwd, "Print working directory"},
    {"cd", cmd_cd, "Change directory: cd [path]"},
    {"touch", cmd_touch, "Create file: touch <filename>"},
    {"cat", cmd_cat, "View file content: cat <filename>"},
    {"edit", cmd_edit, "Edit file: edit <filename>"},
    {"write", cmd_write, "Write to file: write <filename> <content>"},
    {"rm", cmd_rm, "Remove file or empty directory: rm <path>"},
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
    vga_puts("ShOS v1.0\n");  // Changed from MyOS to ShOS
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

void cmd_add(char *args[]) {
    if (!args[1] || !args[2]) {
        vga_puts("Usage: add <num1> <num2>\n");
        return;
    }
    
    int num1 = str_to_int(args[1]);
    int num2 = str_to_int(args[2]);
    int result = num1 + num2;
    
    char result_str[20];
    int_to_str(result, result_str);
    
    vga_puts("Result: ");
    vga_puts(result_str);
    vga_puts("\n");
}

void cmd_subtract(char *args[]) {
    if (!args[1] || !args[2]) {
        vga_puts("Usage: sub <num1> <num2>\n");
        return;
    }
    
    int num1 = str_to_int(args[1]);
    int num2 = str_to_int(args[2]);
    int result = num1 - num2;
    
    char result_str[20];
    int_to_str(result, result_str);
    
    vga_puts("Result: ");
    vga_puts(result_str);
    vga_puts("\n");
}

void cmd_multiply(char *args[]) {
    if (!args[1] || !args[2]) {
        vga_puts("Usage: mul <num1> <num2>\n");
        return;
    }
    
    int num1 = str_to_int(args[1]);
    int num2 = str_to_int(args[2]);
    int result = num1 * num2;
    
    char result_str[20];
    int_to_str(result, result_str);
    
    vga_puts("Result: ");
    vga_puts(result_str);
    vga_puts("\n");
}

void cmd_divide(char *args[]) {
    if (!args[1] || !args[2]) {
        vga_puts("Usage: div <num1> <num2>\n");
        return;
    }
    
    int num1 = str_to_int(args[1]);
    int num2 = str_to_int(args[2]);
    
    if (num2 == 0) {
        vga_puts("Error: Division by zero\n");
        return;
    }
    
    int result = num1 / num2;
    
    char result_str[20];
    int_to_str(result, result_str);
    
    vga_puts("Result: ");
    vga_puts(result_str);
    vga_puts("\n");
}

void cmd_tictactoe(char *args[]) {
    (void)args; // Unused parameter
    tictactoe_game();
}

void cmd_mkdir(char *args[]) {
    if (!args[1]) {
        vga_puts("Usage: mkdir <dirname>\n");
        return;
    }
    fs_mkdir(args[1]);
}

void cmd_ls(char *args[]) {
    fs_ls(args[1]); // args[1] can be NULL for current dir
}

void cmd_pwd(char *args[]) {
    (void)args;
    fs_pwd();
}

void cmd_cd(char *args[]) {
    fs_cd(args[1]); // args[1] can be NULL for home dir
}

void cmd_touch(char *args[]) {
    if (!args[1]) {
        vga_puts("Usage: touch <filename>\n");
        return;
    }
    fs_touch(args[1]);
}

void cmd_cat(char *args[]) {
    if (!args[1]) {
        vga_puts("Usage: cat <filename>\n");
        return;
    }
    fs_cat(args[1]);
}

void cmd_edit(char *args[]) {
    if (!args[1]) {
        vga_puts("Usage: edit <filename>\n");
        return;
    }
    
    // Create file if it doesn't exist
    if (fs_find_file(args[1]) == NULL) {
        fs_touch(args[1]);
    }
    
    editor_nano(args[1]);
}

void cmd_write(char *args[]) {
    if (!args[1] || !args[2]) {
        vga_puts("Usage: write <filename> <content>\n");
        return;
    }
    
    // Create file if it doesn't exist
    if (fs_find_file(args[1]) == NULL) {
        fs_touch(args[1]);
    }
    
    // Combine all arguments after filename as content
    char content[256] = {0};
    for (int i = 2; args[i]; i++) {
        kstrcat(content, args[i]);
        if (args[i + 1]) {
            kstrcat(content, " ");
        }
    }
    
    fs_write(args[1], content);
}

void cmd_rm(char *args[]) {
    if (!args[1]) {
        vga_puts("Usage: rm <file_or_directory>\n");
        return;
    }
    fs_rm(args[1]);
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
    
    vga_puts("ShOS Shell - Type 'help' for available commands\n");
    
    while (1) {
        vga_puts("> ");
        kgets(input, MAX_INPUT);
        execute_command(input);
    }
}