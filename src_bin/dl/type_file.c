#include "modules.h"
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int has_extension(const char *filename);

char *define_type_file(unsigned char d_type, const char *filename) {
  /* Defines type of file/directory */
  switch (d_type) {
  case DT_REG:
    return has_extension(filename) ? "[f]" : "[f]" GREEN;
  case DT_DIR:
    return "[d]" BLUE BOLD;
  case DT_LNK:
    return "[ln]" YELLOW;
  case DT_FIFO:
    return "[pp]";
  case DT_SOCK:
    return "[sck]";
  case DT_CHR:
    return "[chr]";
  case DT_BLK:
    return "[blk]";
  }
}

int has_extension(const char *filename) {
  struct stat st;
  const char *dot = strrchr(filename, '.');
  return (dot != NULL);
}
