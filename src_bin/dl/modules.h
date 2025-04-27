#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

extern int write_all;
extern int show_stat;
extern char *separator;

int print_dir(const char *dir_path);
void statistic(mode_t mode, struct stat st);
char *define_type_file(unsigned char d_type, const char *filename);

#ifndef MODULES_H
#define MODULES_H

#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

#endif
