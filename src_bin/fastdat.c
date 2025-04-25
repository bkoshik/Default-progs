#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
  if (argc < 2) {
    write(STDERR_FILENO, "Usage: ./dat <filename>\n", 25);
    return 1;
  }

  int fd;
  char buffer[BUF_SIZE];
  ssize_t bytes_read;

  for (int i = 1; i < argc; i++) {
    fd = open(argv[i], O_RDONLY);
    if (fd == -1) {
      perror("open");
      return 1;
    }
    printf("\033[1m\t\tFILE: %s\033[0m\n\n", argv[i]);

    while ((bytes_read = read(fd, buffer, BUF_SIZE)) > 0) {
      write(STDOUT_FILENO, buffer, bytes_read);
    }

    if (bytes_read == -1) {
      perror("read");
      close(fd);
      return 1;
    }
    printf("\n\n");
    close(fd);
  }

  return 0;
}
