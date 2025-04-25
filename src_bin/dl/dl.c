#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include "modules.h"

#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

int main(int argc, char *argv[]) {
  int opt;           // options/flags
  int write_all = 0; // print dotfiles or no
  char *separator = "  ";
  int show_stat = 0;
  opterr = 1;

  /* Creating flags */
  while ((opt = getopt(argc, argv, "has:l")) != -1) {
    switch (opt) {
    case 'h':
      dprintf(STDOUT_FILENO, "Usage:\n\tdl [dir] [-a, -s <sep>]\n\n"
                             "\t-a    show all files (incl. dotfiles)\n"
                             "\t-s    custom separator (default '\\n')\n"
                             "\t-l    show stats\n");
      return 0;
    case 'a':
      write_all = 1;
      break;
    case 's':
      separator = optarg;
      break;
    case 'l':
      show_stat = 1;
      separator = "\n";
      break;
    case '?':
      fprintf(stderr, "Unknown option: '-%c'\n", optopt);
    default:
      return 1;
    }
  }

  /* Opens . if there is no arguments else uses argument but no -a */
  const char *dir_path = (optind < argc) ? argv[optind] : ".";
  int num_of_end = print_dir(dir_path, separator, write_all, show_stat);

  return num_of_end;
}

