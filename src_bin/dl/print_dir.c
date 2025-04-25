#include "modules.h"
#include <dirent.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

void processing_dir(struct stat st, struct dirent *dr, DIR *dir,
                    char *separator, int write_all, const char *dir_path,
                    int show_stat);

int print_dir(const char *dir_path, char *separator, int write_all,
              int show_stat) {
  DIR *dir = opendir(dir_path);
  struct dirent *dr;
  struct stat st;

  /* Writes fail message to stderr if there is no dir */
  if (!dir) {
    write(STDERR_FILENO, "opendir failed", 14);
    return 1;
  }

  /* Reads dir */
  processing_dir(st, dr, dir, separator, write_all, dir_path, show_stat);

  /* I think you know what do this func (closes dir) */
  closedir(dir);

  return 0;
}

void processing_dir(struct stat st, struct dirent *dr, DIR *dir,
                    char *separator, int write_all, const char *dir_path,
                    int show_stat) {
  while ((dr = readdir(dir)) != NULL) {
    /* Skips dotfiles without -a */
    if (dr->d_name[0] == '.' && !write_all)
      continue;

    char full_path[PATH_MAX + 1];
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, dr->d_name);

    // Получаем информацию о файле
    if (stat(full_path, &st) == -1) {
      perror("stat failed");
      continue;
    }

    if (show_stat)
      statistic(st.st_mode, st);

    /* I know below this so many write(), but it is for compatibility with POSIX
     * and low-level */

    /* Writes type */
    char *type = define_type_file(dr->d_type, dr->d_name);
    write(STDOUT_FILENO, type, strlen(type));

    write(STDOUT_FILENO, dr->d_name, strlen(dr->d_name));
    if (dr->d_type == DT_DIR) /* Writes formatted dir */
      write(STDOUT_FILENO, "/", 1);
    write(STDOUT_FILENO, RESET, strlen(RESET));
    write(STDOUT_FILENO, separator, strlen(separator));
  }
}
