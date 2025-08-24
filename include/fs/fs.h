// fs.h
#ifndef FS_H
#define FS_H
#include <stddef.h>

#define MAX_FILENAME_LEN 32
#define MAX_PATH_LEN 128
#define MAX_FILES 100
#define MAX_DIRS 50
#define MAX_FILE_SIZE 1024  // 1KB max file size

// Define size_t if not available
#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

typedef enum {
    TYPE_FILE,
    TYPE_DIRECTORY
} fs_node_type;

typedef struct fs_node {
    char name[MAX_FILENAME_LEN];
    fs_node_type type;
    char *data;
    size_t size;
    size_t capacity;
    struct fs_node *parent;
    struct fs_node *children;
    struct fs_node *next; // for linked list of siblings
} fs_node;

// Filesystem functions
void fs_init(void);
int fs_mkdir(const char *path);
int fs_touch(const char *filename);  // Create empty file
int fs_write(const char *filename, const char *content);  // Write to file
int fs_cat(const char *filename);  // Read file content
int fs_ls(const char *path);
int fs_pwd(void);
int fs_cd(const char *path);
int fs_rm(const char *path);
int fs_remove_node(fs_node *node);
fs_node *fs_get_current_dir(void);
fs_node *fs_find_file(const char *filename);  // Find file in current dir

#endif