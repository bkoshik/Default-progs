#include <string.h>
#include <unistd.h>

int main(void) {
  char cwd[100];

  if (getcwd(cwd, sizeof(cwd)) != NULL)
    write(STDOUT_FILENO, cwd, strlen(cwd));

  return 0;
}
