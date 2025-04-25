#include <dirent.h>
#include <sys/stat.h>

int print_dir(const char *dir_path, char *separator, int write_all, int show_stat);
void processing_dir(struct stat st, struct dirent *dr, DIR *dir, char *separator, int write_all, const char *dir_path, int show_stat);
void statistic(mode_t mode, struct stat st);
char *define_type_file(unsigned char d_type, const char *filename);
