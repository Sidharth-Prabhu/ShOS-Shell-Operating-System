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
    root_dir.capacity = 0;
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
    node->capacity = 0;
    node->parent = current_dir;
    node->children = NULL;
    node->next = NULL;
    
    return node;
}

fs_node *fs_find_file(const char *filename) {
    fs_node *current = current_dir->children;
    while (current != NULL) {
        if (kstreq(current->name, filename) && current->type == TYPE_FILE) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

fs_node *fs_find_dir(const char *dirname) {
    fs_node *current = current_dir->children;
    while (current != NULL) {
        if (kstreq(current->name, dirname) && current->type == TYPE_DIRECTORY) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int fs_mkdir(const char *path) {
    if (kstrlen(path) == 0 || kstrlen(path) >= MAX_FILENAME_LEN) {
        return -1; // Invalid path
    }
    
    // Check if directory already exists
    if (fs_find_dir(path) != NULL) {
        vga_puts("Directory already exists: ");
        vga_puts(path);
        vga_puts("\n");
        return -1;
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

int fs_touch(const char *filename) {
    if (kstrlen(filename) == 0 || kstrlen(filename) >= MAX_FILENAME_LEN) {
        return -1; // Invalid filename
    }
    
    // Check if file already exists
    if (fs_find_file(filename) != NULL) {
        vga_puts("File already exists: ");
        vga_puts(filename);
        vga_puts("\n");
        return -1;
    }
    
    // Create new file
    fs_node *new_file = fs_create_node(filename, TYPE_FILE);
    if (new_file == NULL) {
        vga_puts("Cannot create file: filesystem full\n");
        return -1;
    }
    
    // Add to current directory's children
    new_file->next = current_dir->children;
    current_dir->children = new_file;
    
    vga_puts("Created file: ");
    vga_puts(filename);
    vga_puts("\n");
    return 0;
}

int fs_write(const char *filename, const char *content) {
    fs_node *file = fs_find_file(filename);
    if (file == NULL) {
        vga_puts("File not found: ");
        vga_puts(filename);
        vga_puts("\n");
        return -1;
    }
    
    // For simplicity, we'll just store the content pointer
    // In a real OS, we'd copy the content to allocated memory
    file->data = (char*)content;
    file->size = kstrlen(content);
    
    vga_puts("Written to ");
    vga_puts(filename);
    vga_puts("\n");
    return 0;
}

int fs_cat(const char *filename) {
    fs_node *file = fs_find_file(filename);
    if (file == NULL) {
        vga_puts("File not found: ");
        vga_puts(filename);
        vga_puts("\n");
        return -1;
    }
    
    if (file->size == 0 || file->data == NULL) {
        vga_puts("File is empty: ");
        vga_puts(filename);
        vga_puts("\n");
        return 0;
    }
    
    vga_puts("Contents of ");
    vga_puts(filename);
    vga_puts(":\n");
    
    // Display file content
    for (unsigned int i = 0; i < file->size; i++) {
        vga_putc(file->data[i]);
    }
    vga_puts("\n");
    
    return 0;
}

int fs_ls(const char *path) {
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
    
    while (node != NULL && node != &root_dir) {
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
    fs_node *dir = fs_find_dir(path);
    if (dir != NULL) {
        current_dir = dir;
        return 0;
    }
    
    vga_puts("Directory not found: ");
    vga_puts(path);
    vga_puts("\n");
    return -1;
}

// Helper function to remove a node from its parent's children list
int fs_remove_node(fs_node *node) {
    if (node == NULL || node->parent == NULL) {
        return -1;
    }
    
    // Special case: node is the first child
    if (node->parent->children == node) {
        node->parent->children = node->next;
        return 0;
    }
    
    // Find the node in the children list
    fs_node *current = node->parent->children;
    while (current != NULL && current->next != node) {
        current = current->next;
    }
    
    if (current != NULL) {
        current->next = node->next;
        return 0;
    }
    
    return -1; // Node not found in parent's children
}

int fs_rm(const char *path) {
    if (path == NULL || kstrlen(path) == 0) {
        vga_puts("Usage: rm <file_or_directory>\n");
        return -1;
    }
    
    // Check if it's a file
    fs_node *file = fs_find_file(path);
    if (file != NULL) {
        if (fs_remove_node(file) == 0) {
            vga_puts("Removed file: ");
            vga_puts(path);
            vga_puts("\n");
            return 0;
        }
    }
    
    // Check if it's a directory
    fs_node *dir = fs_find_dir(path);
    if (dir != NULL) {
        // Check if directory is empty
        if (dir->children != NULL) {
            vga_puts("Cannot remove directory: ");
            vga_puts(path);
            vga_puts(" is not empty\n");
            return -1;
        }
        
        // Don't allow removing current directory
        if (dir == current_dir) {
            vga_puts("Cannot remove current directory\n");
            return -1;
        }
        
        // Don't allow removing root directory
        if (dir == &root_dir) {
            vga_puts("Cannot remove root directory\n");
            return -1;
        }
        
        if (fs_remove_node(dir) == 0) {
            vga_puts("Removed directory: ");
            vga_puts(path);
            vga_puts("\n");
            return 0;
        }
    }
    
    vga_puts("File or directory not found: ");
    vga_puts(path);
    vga_puts("\n");
    return -1;
}

fs_node *fs_get_current_dir(void) {
    return current_dir;
}