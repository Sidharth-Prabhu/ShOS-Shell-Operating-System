// fs.h
#ifndef FS_H
#define FS_H

#include <stddef.h>  // Add this line for size_t

#define MAX_FILENAME_LEN 32
#define MAX_PATH_LEN 128
#define MAX_FILES 100
#define MAX_DIRS 50

typedef enum {
    TYPE_FILE,
    TYPE_DIRECTORY
} fs_node_type;

typedef struct fs_node {
    char name[MAX_FILENAME_LEN];
    fs_node_type type;
    char *data;
    size_t size;
    struct fs_node *parent;
    struct fs_node *children;
    struct fs_node *next; // for linked list of siblings
} fs_node;

// Filesystem functions
void fs_init(void);
int fs_mkdir(const char *path);
int fs_ls(const char *path);
int fs_pwd(void);
int fs_cd(const char *path);
fs_node *fs_get_current_dir(void);

#endif