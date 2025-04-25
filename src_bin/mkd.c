#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (mkdir(argv[1], 0775) == -1)
    if (errno == EEXIST) {
      write(STDERR_FILENO, "Directory is already exist", 26);
      return 1;
    }

  return 0;
}
