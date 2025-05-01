#include "modules.h"
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void perms(char *perms, mode_t mode, char *target_link, const char *dir_path);
char *format_size(off_t size);
const char *format_color(Color_Perms color);
void apply_permissions_color(char perms[], Permission perm_color, char *output);

void statistic(mode_t mode, struct stat st, struct dirent *dr,
               const char *dir_path, char *target_link) {
  char permisions[11] = ".---------\0";
  perms(permisions, mode, target_link, dir_path);

  char *size = format_size(st.st_size);

  char color_buf_perm[128];
  apply_permissions_color(permisions, color_permission(), color_buf_perm);

  struct passwd *pw = getpwuid(st.st_uid);
  struct group *gr = getgrgid(st.st_gid);

  char *pw_name = pw ? pw->pw_name : "unknown";
  char *gr_name = gr ? gr->gr_name : "unknown";

  User_Group user_color = color_user_group();
  char user_color_buf[1024];

  snprintf(user_color_buf, sizeof(user_color_buf),
           "\033[38;5;%dm%s\033[0m : \033[%dm%s\033[0m", user_color.user,
           pw_name, user_color.group, gr_name);

  write(STDOUT_FILENO, color_buf_perm, strlen(color_buf_perm));
  write(STDOUT_FILENO, " ", 1);
  write(STDOUT_FILENO, user_color_buf, strlen(user_color_buf));
  write(STDOUT_FILENO, " ", 1);
  write(STDOUT_FILENO, size, strlen(size));
  write(STDOUT_FILENO, " ", 1);
}

void perms(char *perms, mode_t mode, char *target_link, const char *dir_path) {
  if (S_ISDIR(mode))
    perms[0] = 'd';
  else if (S_ISLNK(mode)) {
    perms[0] = 'l';
    ssize_t len = readlink(dir_path, target_link, 1023);
    if (len == -1) {
      perror("readlink");
      exit(1);
    }
    target_link[len] = '\0';
  } else if (S_ISBLK(mode))
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
  int type_color = perm_color.no_access;
  switch (perms[0]) {
  case 'd':
    type_color = perm_color.dir;
    break;
  case '.':
    type_color = perm_color.file;
    break;
  case 'l':
    type_color = perm_color.symlink;
    break;
  case 'c':
    type_color = perm_color.char_device;
    break;
  case 'b':
    type_color = perm_color.block_device;
    break;
  case 's':
    type_color = perm_color.socket;
    break;
  case 'p':
    type_color = perm_color.pipe;
    break;
  default:
    type_color = perm_color.no_access;
  }

  snprintf(output, 16, "\033[%d;%dm%c\033[0m", type_color,
           perms[0] != '.' ? 1 : 0, perms[0]);

  for (int i = 1; i < 10; ++i) {
    int color_code = 0;
    if (perms[i] == 'r') {
      color_code = (i % 3 == 1) ? perm_color.read : perm_color.no_access;
    } else if (perms[i] == 'w') {
      color_code = (i % 3 == 2) ? perm_color.write : perm_color.no_access;
    } else if (perms[i] == 'x') {
      color_code = (i % 3 == 0) ? perm_color.exec : perm_color.no_access;
    } else {
      color_code = perm_color.no_access;
    }

    // Добавляем символ с цветом
    snprintf(output + strlen(output), 256, "\033[%dm%c\033[0m", color_code,
             perms[i]);
  }
}
