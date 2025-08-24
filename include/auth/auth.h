// auth.h
#ifndef AUTH_H
#define AUTH_H

#define MAX_USERNAME_LEN 20
#define MAX_PASSWORD_LEN 20
#define MAX_USERS 10
#define AUTH_FILE "users.dat"

typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    int is_admin;
} User;

void auth_init(void);
int auth_login(void);
int auth_register(void);
int auth_check_credentials(const char *username, const char *password);
int auth_save_users(void);
int auth_load_users(void);

#endif