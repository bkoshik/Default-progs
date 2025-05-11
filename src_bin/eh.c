#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    write(STDERR_FILENO, "Usage: eh <something>\n", 23);
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    write(STDOUT_FILENO, argv[i], strlen(argv[i]));
    write(STDOUT_FILENO, " ", 1);
  }
  write(STDOUT_FILENO, "\n", 1);

  return 0;
}
