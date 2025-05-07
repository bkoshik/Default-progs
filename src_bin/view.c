#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define COMMAND (argv[1])
#define TIME ((argc > 2) ? atoi(argv[2]) : 1)
#define CLEAR "\033[2J\033[H"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    char *instruction = "Usage: view [command] {time}";
    write(STDOUT_FILENO, instruction, strlen(instruction));
    return 1;
  }

  while (1) {
    write(STDOUT_FILENO, CLEAR, strlen(CLEAR));
    pid_t pid = fork();

    if (pid == 0) {
      char *args_execv[2] = {COMMAND, NULL};
      execvp(COMMAND, args_execv);
      perror(argv[0]);
      return 1;
    } else if (pid > 0) {
      wait(NULL);
      sleep(TIME);
    }
  }

  return 0;
}
