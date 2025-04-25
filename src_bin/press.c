#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    write(STDERR_FILENO, "Usage: press <filename>", 20);
    return 1;
  }

  int fd = open(argv[1], O_CREAT | O_EXCL | O_WRONLY, 0644);
  if (fd == -1) {
    perror("press");
    return 1;
  }

  if (argc > 2)
    for (int i = 2; i < argc; i++) {
      write(fd, argv[i], strlen(argv[i]));
      write(fd, " ", 1);
    }

  close(fd);

  return 0;
}
