// fs.c
#include "fs.h"
#include "vga.h"
#include "klib.h"

static fs_node root_dir;
static fs_node *current_dir;
static fs_node nodes[MAX_DIRS + MAX_FILES];
static int node_count = 0;

void fs_init(void) {
    // Initialize root directory
    kstrcpy(root_dir.name, "/");
    root_dir.type = TYPE_DIRECTORY;
    root_dir.data = NULL;
    root_dir.size = 0;
    root_dir.parent = NULL;
    root_dir.children = NULL;
    root_dir.next = NULL;
    
    current_dir = &root_dir;
    node_count = 1;
}

fs_node *fs_create_node(const char *name, fs_node_type type) {
    if (node_count >= MAX_DIRS + MAX_FILES) {
        return NULL;
    }
    
    fs_node *node = &nodes[node_count++];
    kstrcpy(node->name, name);
    node->type = type;
    node->data = NULL;
    node->size = 0;
    node->parent = current_dir;
    node->children = NULL;
    node->next = NULL;
    
    return node;
}

int fs_mkdir(const char *path) {
    if (kstrlen(path) == 0 || kstrlen(path) >= MAX_FILENAME_LEN) {
        return -1; // Invalid path
    }
    
    // Check if directory already exists
    fs_node *current = current_dir->children;
    while (current != NULL) {
        if (kstreq(current->name, path) && current->type == TYPE_DIRECTORY) {
            vga_puts("Directory already exists: ");
            vga_puts(path);
            vga_puts("\n");
            return -1;
        }
        current = current->next;
    }
    
    // Create new directory
    fs_node *new_dir = fs_create_node(path, TYPE_DIRECTORY);
    if (new_dir == NULL) {
        vga_puts("Cannot create directory: filesystem full\n");
        return -1;
    }
    
    // Add to current directory's children
    new_dir->next = current_dir->children;
    current_dir->children = new_dir;
    
    vga_puts("Created directory: ");
    vga_puts(path);
    vga_puts("\n");
    return 0;
}

int fs_ls(const char *path) {
    // Remove the unused variable
    // fs_node *dir = current_dir;  // This line is commented out
    
    // If path is provided, we'd need to navigate to it
    // For simplicity, just list current directory
    if (path != NULL && kstrlen(path) > 0) {
        vga_puts("ls with path not implemented yet\n");
        return -1;
    }
    
    vga_puts("Contents of ");
    if (current_dir == &root_dir) {
        vga_puts("/");
    } else {
        vga_puts(current_dir->name);
    }
    vga_puts(":\n");
    
    fs_node *current = current_dir->children;
    int count = 0;
    
    while (current != NULL) {
        vga_puts("  ");
        if (current->type == TYPE_DIRECTORY) {
            vga_puts("[DIR]  ");
        } else {
            vga_puts("[FILE] ");
        }
        vga_puts(current->name);
        vga_puts("\n");
        current = current->next;
        count++;
    }
    
    if (count == 0) {
        vga_puts("  (empty)\n");
    }
    
    return 0;
}

int fs_pwd(void) {
    vga_puts("Current directory: ");
    
    // Build path by traversing up the tree
    char path[MAX_PATH_LEN] = "";
    fs_node *node = current_dir;
    
    while (node != NULL) {
        char temp[MAX_PATH_LEN];
        kstrcpy(temp, path);
        kstrcpy(path, "/");
        kstrcat(path, node->name);
        kstrcat(path, temp);
        node = node->parent;
    }
    
    if (kstrlen(path) == 0) {
        kstrcpy(path, "/");
    }
    
    vga_puts(path);
    vga_puts("\n");
    return 0;
}

int fs_cd(const char *path) {
    if (path == NULL || kstrlen(path) == 0) {
        // cd to home (root)
        current_dir = &root_dir;
        return 0;
    }
    
    if (kstreq(path, "..")) {
        // Move to parent directory
        if (current_dir->parent != NULL) {
            current_dir = current_dir->parent;
        }
        return 0;
    }
    
    if (kstreq(path, ".")) {
        // Stay in current directory
        return 0;
    }
    
    if (kstreq(path, "/")) {
        // cd to root
        current_dir = &root_dir;
        return 0;
    }
    
    // Look for directory in current directory
    fs_node *current = current_dir->children;
    while (current != NULL) {
        if (kstreq(current->name, path) && current->type == TYPE_DIRECTORY) {
            current_dir = current;
            return 0;
        }
        current = current->next;
    }
    
    vga_puts("Directory not found: ");
    vga_puts(path);
    vga_puts("\n");
    return -1;
}

fs_node *fs_get_current_dir(void) {
    return current_dir;
}