#include "modules.h"
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef enum {
  DarkGreen = 32, // ANSI
  DarkYellow = 33,
  DarkRed = 31,
  AnsiValue_5 = 5,
  Grey = 2,
  AnsiValue_6 = 6,
  DarkCyan = 36,
  Cyan = 36,

  AnsiValue_40 = 40,
  AnsiValue_44 = 44,
  AnsiValue_33 = 33,
  AnsiValue_172 = 172
} Color;

typedef struct {
  Color read;
  Color write;
  Color exec;
  Color exec_sticky;
  Color no_access;
  Color octal;
  Color acl;
  Color context;

  Color file;
  Color dir;
  Color symlink;
  Color pipe;         // darkturquoise
  Color block_device; // darkturquoise
  Color char_device;  // orange3
  Color socket;       // darkturquoise
  Color special;      // darkturquoise
} Permission;

Permission color_permission() {
  Permission p;
  p.read = DarkGreen;
  p.write = DarkYellow;
  p.exec = DarkRed;
  p.exec_sticky = AnsiValue_5;
  p.no_access = Grey;
  p.octal = AnsiValue_6;
  p.acl = DarkCyan;
  p.context = Cyan;

  p.file = AnsiValue_40;
  p.dir = AnsiValue_33;
  p.symlink = AnsiValue_44;
  p.pipe = AnsiValue_44;         // darkturquoise
  p.block_device = AnsiValue_44; // darkturquoise
  p.char_device = AnsiValue_172; // orange3
  p.socket = AnsiValue_44;       // darkturquoise
  p.special = AnsiValue_44;      // darkturquoise
  return p;
}

char *format_size(off_t size);
const char *format_color(Color color);
void apply_permissions_color(char perms[], Permission perm_color, char *output);

void statistic(mode_t mode, struct stat st) {
  char perms[11] = ".---------\0";

  if (S_ISDIR(mode))
    perms[0] = 'd';
  else if (S_ISLNK(mode))
    perms[0] = 'l';
  else if (S_ISBLK(mode))
    perms[0] = 'b';
  else if (S_ISCHR(mode))
    perms[0] = 'c';
  else if (S_ISFIFO(mode))
    perms[0] = 'p';
  else if (S_ISSOCK(mode))
    perms[0] = 's';

  // User permisions rwx
  if (mode & S_IRUSR)
    perms[1] = 'r';
  if (mode & S_IWUSR)
    perms[2] = 'w';
  if (mode & S_IXUSR)
    perms[3] = 'x';

  // Group permisions rwx
  if (mode & S_IRGRP)
    perms[4] = 'r';
  if (mode & S_IWGRP)
    perms[5] = 'w';
  if (mode & S_IXGRP)
    perms[6] = 'x';

  // Other permisions rwx
  if (mode & S_IROTH)
    perms[7] = 'r';
  if (mode & S_IWOTH)
    perms[8] = 'w';
  if (mode & S_IXOTH)
    perms[9] = 'x';

  char *size = format_size(st.st_size);

  char color_buf_perm[128];
  Permission perm_color = color_permission();
  apply_permissions_color(perms, perm_color, color_buf_perm);

  struct passwd *pw = getpwuid(st.st_uid);
  struct group *gr = getgrgid(st.st_gid);

  char *pw_name = pw ? pw->pw_name : "unknown";
  char *gr_name = gr ? gr->gr_name : "unknown";

  write(STDOUT_FILENO, color_buf_perm, strlen(color_buf_perm));
  write(STDOUT_FILENO, " ", 1);
  write(STDOUT_FILENO, pw_name, strlen(pw_name));
  write(STDOUT_FILENO, ":", 1);
  write(STDOUT_FILENO, gr_name, strlen(gr_name));
  write(STDOUT_FILENO, " ", 1);
  write(STDOUT_FILENO, size, strlen(size));
  write(STDOUT_FILENO, " ", 1);
}

char *format_size(off_t size) {
  static char buf[24];
  if (size >= 1024 * 1024 * 1024) {
    sprintf(buf, "%.1f G", (double)size / (1024 * 1024 * 1024));
  } else if (size >= 1024 * 1024) {
    sprintf(buf, "%.1f M", (double)size / (1024 * 1024));
  } else if (size >= 1024) {
    sprintf(buf, "%.1f K", (double)size / 1024);
  } else {
    sprintf(buf, "%ld B", (long)size);
  }
  return buf;
}

void apply_permissions_color(char perms[], Permission perm_color,
                             char *output) {
  snprintf(
      output, 256,
      "\033[%dm%c\033[0m"
      "\033[%dm%c\033[0m"
      "\033[%dm%c\033[0m"
      "\033[%dm%c\033[0m"
      "\033[%dm%c\033[0m"
      "\033[%dm%c\033[0m"
      "\033[%dm%c\033[0m"
      "\033[%dm%c\033[0m"
      "\033[%dm%c\033[0m",
      (perms[1] == 'r') ? perm_color.read : perm_color.no_access, perms[1],
      (perms[2] == 'w') ? perm_color.write : perm_color.no_access, perms[2],
      (perms[3] == 'x') ? perm_color.exec : perm_color.no_access, perms[3],
      (perms[4] == 'r') ? perm_color.read : perm_color.no_access, perms[4],
      (perms[5] == 'w') ? perm_color.write : perm_color.no_access, perms[5],
      (perms[6] == 'x') ? perm_color.exec : perm_color.no_access, perms[6],
      (perms[7] == 'r') ? perm_color.read : perm_color.no_access, perms[7],
      (perms[8] == 'w') ? perm_color.write : perm_color.no_access, perms[8],
      (perms[9] == 'x') ? perm_color.exec : perm_color.no_access, perms[9]);
}
