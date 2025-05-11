#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  for (int i = 1; i < argc; i++)
    if (mkdir(argv[i], 0775) == -1)
      if (errno == EEXIST) {
        write(STDERR_FILENO, "Directory is already exist", 26);
        return 1;
      }

  return 0;
}
