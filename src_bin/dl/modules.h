#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

extern int write_all;
extern int show_stat;
extern char *separator;

int print_dir(const char *dir_path);
void statistic(mode_t mode, struct stat st, struct dirent *dr,
               const char *dir_path, char *target_link);
char *define_type_file(unsigned char d_type, const char *filename);

#ifndef MODULES_H
#define MODULES_H

#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

#endif

typedef enum {
  AnsiValue_230 = 230,
  AnsiValue_93 = 93,
} Color_User;

typedef struct {
  Color_User user;
  Color_User group;
} User_Group;

User_Group color_user_group();

typedef enum {
  DarkGreen = 32, // ANSI
  DarkYellow = 33,
  DarkRed = 31,
  AnsiValue_5 = 5,
  Grey = 2,
  AnsiValue_6 = 6,
  DarkCyan = 36,
  Cyan = 36,

  AnsiValue_8 = 8,
  AnsiValue_40 = 40,
  AnsiValue_44 = 44,
  AnsiValue_34 = 34,
  AnsiValue_172 = 172
} Color_Perms;

typedef struct {
  Color_Perms read;
  Color_Perms write;
  Color_Perms exec;
  Color_Perms exec_sticky;
  Color_Perms no_access;
  Color_Perms octal;
  Color_Perms acl;
  Color_Perms context;

  Color_Perms file;
  Color_Perms dir;
  Color_Perms symlink;
  Color_Perms pipe;         // darkturquoise
  Color_Perms block_device; // darkturquoise
  Color_Perms char_device;  // orange3
  Color_Perms socket;       // darkturquoise
  Color_Perms special;      // darkturquoise
} Permission;

Permission color_permission();
