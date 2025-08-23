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
void cmd_add(char *args[]);
void cmd_subtract(char *args[]);
void cmd_multiply(char *args[]);
void cmd_divide(char *args[]);

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
    {0, 0, 0} // End marker
};

// String to integer conversion
int str_to_int(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;
    
    // Handle negative numbers
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }
    
    // Convert each character to digit
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
            i++;
        } else {
            return 0; // Invalid character
        }
    }
    
    return result * sign;
}

// Integer to string conversion
void int_to_str(int num, char *str) {
    int i = 0;
    int is_negative = 0;
    
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    // Handle zero case
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    
    // Convert digits in reverse order
    while (num != 0) {
        int digit = num % 10;
        str[i++] = '0' + digit;
        num = num / 10;
    }
    
    // Add negative sign if needed
    if (is_negative) {
        str[i++] = '-';
    }
    
    str[i] = '\0';
    
    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

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