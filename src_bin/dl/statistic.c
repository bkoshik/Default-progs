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

void statistic(mode_t mode, struct stat st) {
  char perms[11] = ".---------";

  if (S_ISDIR(mode))
    perms[0] = 'd';
  if (mode & S_IRUSR)
    perms[1] = 'r';
  if (mode & S_IWUSR)
    perms[2] = 'w';
  if (mode & S_IXUSR)
    perms[3] = 'x';
  if (mode & S_IRGRP)
    perms[4] = 'r';
  if (mode & S_IWGRP)
    perms[5] = 'w';
  if (mode & S_IXGRP)
    perms[6] = 'x';
  if (mode & S_IROTH)
    perms[7] = 'r';
  if (mode & S_IWOTH)
    perms[8] = 'w';
  if (mode & S_IXOTH)
    perms[9] = 'x';

  write(STDOUT_FILENO, perms, 10);
  write(STDOUT_FILENO, " ", 1);

  struct passwd *pw = getpwuid(st.st_uid);
  if (pw) {
    write(STDOUT_FILENO, pw->pw_name, strlen(pw->pw_name));
  } else {
    fprintf(stderr, "%d", (int)st.st_uid); // fallback
  }
  write(STDOUT_FILENO, ":", 1);

  struct group *gr = getgrgid(st.st_gid);
  if (gr) {
    write(STDOUT_FILENO, gr->gr_name, strlen(gr->gr_name));
  } else {
    fprintf(stderr, "%d", (int)st.st_gid);
  }
  write(STDOUT_FILENO, " ", 1);

  char size_buf[18];
  snprintf(size_buf, sizeof(size_buf), "%ld B  ", st.st_size);
  write(STDOUT_FILENO, size_buf, strlen(size_buf));
}
