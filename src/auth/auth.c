// auth.c
#include "auth.h"
#include "vga.h"
#include "klib.h"
#include "fs.h"

static User users[MAX_USERS];
static int user_count = 0;

// Simulated file operations (in real OS, these would use actual file system)
void auth_init(void) {
    // Try to load users from file
    if (auth_load_users() == 0) {
        // If no users file exists, create default users
        kstrcpy(users[0].username, "admin");
        kstrcpy(users[0].password, "password");
        users[0].is_admin = 1;
        
        kstrcpy(users[1].username, "user");
        kstrcpy(users[1].password, "12345");
        users[1].is_admin = 0;
        
        user_count = 2;
        auth_save_users(); // Save default users
    }
}

// Simulated file save operation
int auth_save_users(void) {
    // In a real OS, this would write to actual disk
    // For now, we'll create a virtual file in our RAM filesystem
    
    // Create a string representation of all users
    char file_content[1024] = {0};
    
    for (int i = 0; i < user_count; i++) {
        kstrcat(file_content, users[i].username);
        kstrcat(file_content, ":");
        kstrcat(file_content, users[i].password);
        kstrcat(file_content, ":");
        
        char admin_str[4];
        int_to_str(users[i].is_admin, admin_str);
        kstrcat(file_content, admin_str);
        kstrcat(file_content, "\n");
    }
    
    // Create or overwrite the users file
    fs_node *file = fs_find_file(AUTH_FILE);
    if (file == NULL) {
        fs_touch(AUTH_FILE);
        file = fs_find_file(AUTH_FILE);
    }
    
    if (file != NULL) {
        file->data = file_content;
        file->size = kstrlen(file_content);
        return 1; // Success
    }
    
    return 0; // Failure
}

// Simulated file load operation
int auth_load_users(void) {
    // Look for users file
    fs_node *file = fs_find_file(AUTH_FILE);
    if (file == NULL || file->data == NULL) {
        return 0; // File doesn't exist
    }
    
    // Parse file content
    char *content = file->data;
    // int line_num = 0;
    char *line_start = content;
    
    user_count = 0;
    
    while (*content && user_count < MAX_USERS) {
        if (*content == '\n') {
            *content = '\0'; // Terminate line
            
            // Parse line: username:password:is_admin
            char *parts[3];
            int part_count = 0;
            char *token = line_start;
            
            while (token && part_count < 3) {
                parts[part_count++] = token;
                token = kstrchr(token, ':');
                if (token) {
                    *token = '\0';
                    token++;
                }
            }
            
            if (part_count == 3) {
                // Store user data
                kstrcpy(users[user_count].username, parts[0]);
                kstrcpy(users[user_count].password, parts[1]);
                users[user_count].is_admin = str_to_int(parts[2]);
                user_count++;
            }
            
            line_start = content + 1;
        }
        content++;
    }
    
    return user_count > 0 ? 1 : 0;
}

int auth_check_credentials(const char *username, const char *password) {
    for (int i = 0; i < user_count; i++) {
        if (kstreq(users[i].username, username) && 
            kstreq(users[i].password, password)) {
            return 1; // Authentication successful
        }
    }
    return 0; // Authentication failed
}

int auth_login(void) {
    char username[MAX_USERNAME_LEN] = {0};
    char password[MAX_PASSWORD_LEN] = {0};
    int attempts = 0;
    
    vga_clear();
    
    while (attempts < 3) {
        vga_puts("ShOS Login\n");
        vga_puts("==========\n\n");
        
        // Get username
        vga_puts("Username: ");
        kgets(username, MAX_USERNAME_LEN);
        
        // Get password (with masking)
        vga_puts("Password: ");
        int i = 0;
        while (i < MAX_PASSWORD_LEN - 1) {
            char c = kgetchar();
            
            if (c == '\n') {
                break;
            } else if (c == '\b' && i > 0) {
                i--;
                vga_putc('\b');
                vga_putc(' ');
                vga_putc('\b');
            } else if (c >= 32 && c <= 126) {
                password[i++] = c;
                vga_putc('*'); // Mask password input
            }
        }
        password[i] = '\0';
        vga_putc('\n');
        
        // Check credentials
        if (auth_check_credentials(username, password)) {
            vga_puts("\nLogin successful! Welcome, ");
            vga_puts(username);
            vga_puts("!\n\n");
            
            // Show welcome message with some delay
            for (volatile int j = 0; j < 2000000; j++);
            
            return 1;
        } else {
            attempts++;
            vga_puts("\nInvalid credentials. Attempts remaining: ");
            char attempts_str[4];
            int_to_str(3 - attempts, attempts_str);
            vga_puts(attempts_str);
            vga_puts("\n\n");
            
            // Clear input fields
            username[0] = '\0';
            password[0] = '\0';
            
            // Add delay before clearing screen
            for (volatile int j = 0; j < 1000000; j++);
            
            if (attempts < 3) {
                vga_clear();
            }
        }
    }
    
    vga_puts("Too many failed attempts. System halted.\n");
    return 0;
}

int auth_register(void) {
    if (user_count >= MAX_USERS) {
        vga_puts("Maximum user limit reached.\n");
        for (volatile int j = 0; j < 1000000; j++);
        return 0;
    }
    
    char username[MAX_USERNAME_LEN] = {0};
    char password[MAX_PASSWORD_LEN] = {0};
    char confirm[MAX_PASSWORD_LEN] = {0};
    
    vga_clear();
    vga_puts("ShOS User Registration\n");
    vga_puts("======================\n\n");
    
    // Get username
    vga_puts("Choose username: ");
    kgets(username, MAX_USERNAME_LEN);
    
    // Check if username already exists
    for (int i = 0; i < user_count; i++) {
        if (kstreq(users[i].username, username)) {
            vga_puts("Username already exists.\n");
            for (volatile int j = 0; j < 1000000; j++);
            return 0;
        }
    }
    
    // Get password
    vga_puts("Choose password: ");
    int i = 0;
    while (i < MAX_PASSWORD_LEN - 1) {
        char c = kgetchar();
        if (c == '\n') break;
        if (c == '\b' && i > 0) {
            i--;
            vga_putc('\b');
            vga_putc(' ');
            vga_putc('\b');
        } else if (c >= 32 && c <= 126) {
            password[i++] = c;
            vga_putc('*');
        }
    }
    password[i] = '\0';
    vga_putc('\n');
    
    // Confirm password
    vga_puts("Confirm password: ");
    i = 0;
    while (i < MAX_PASSWORD_LEN - 1) {
        char c = kgetchar();
        if (c == '\n') break;
        if (c == '\b' && i > 0) {
            i--;
            vga_putc('\b');
            vga_putc(' ');
            vga_putc('\b');
        } else if (c >= 32 && c <= 126) {
            confirm[i++] = c;
            vga_putc('*');
        }
    }
    confirm[i] = '\0';
    vga_putc('\n');
    
    // Check if passwords match
    if (!kstreq(password, confirm)) {
        vga_puts("Passwords do not match.\n");
        for (volatile int j = 0; j < 1000000; j++);
        return 0;
    }
    
    // Create new user
    kstrcpy(users[user_count].username, username);
    kstrcpy(users[user_count].password, password);
    users[user_count].is_admin = 0;
    user_count++;
    
    // Save users to persistent storage
    auth_save_users();
    
    vga_puts("\nRegistration successful! Press any key to continue...\n");
    
    // Wait for any key press
    kgetchar();
    
    return 1;
}