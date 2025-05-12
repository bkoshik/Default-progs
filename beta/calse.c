#include "calse/find_default_command.h"
#include "calse/getprompt.h"
#include "calse/pass.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <pwd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wordexp.h>

#define MAXNAMEUSER 1024
#define MAXPROMPT 2056
#define MAXCOMMAND 4096

int main(void) {
  signal(SIGINT, SIG_IGN);

  while (1) {
    /* Get prompt [user@host (~)] $> */
    char prompt[MAXPROMPT] = {0};

    if (getprompt(prompt, MAXPROMPT, MAXNAMEUSER) == -1) {
      perror("getprompt");
      return 1;
    }

    char *command = readline(prompt);

    if (command == NULL || *command == '\0') {
      free(command);
      continue;
    }

    add_history(command);

    char *saveptr;
    char *fcommand = strtok_r(command, ";", &saveptr);

    for (; fcommand != NULL; fcommand = strtok_r(NULL, ";", &saveptr)) {
      wordexp_t p;
      if (wordexp(fcommand, &p, 0) != 0) {
        fprintf(stderr, "wordexp error\n");
        fcommand = strtok_r(NULL, ";", &saveptr);
        continue;
      }

      if (p.we_wordc == 0) {
        wordfree(&p);
        fcommand = strtok_r(NULL, ";", &saveptr);
        continue;
      }

      /* 1. Default/Builtin command */
      int fdc_return = find_default_command(p.we_wordv, p.we_wordc);
      if (fdc_return == 0)
        continue;
      else if (fdc_return == 1)
        break;
      else if (fdc_return == 2)
        exit(0);
      else if (fdc_return == -1)
        perror(p.we_wordv[0]);
      else if (fdc_return == -2)
        pass();

      /* 2. If argv[0] is a directory, do cd into it */
      struct stat sb;
      if (stat(p.we_wordv[0], &sb) == 0)
        if (S_ISDIR(sb.st_mode)) {
          chdir(p.we_wordv[0]);
          wordfree(&p);
          continue;
        }

      /* 3. External command with fork/execvp */
      pid_t pid = fork();

      if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        execvp(p.we_wordv[0], p.we_wordv);

        if (errno == ENOENT)
          fprintf(stderr, "calse: command not found: %s", p.we_wordv[0]);
        else if (errno == EACCES)
          fprintf(stderr, "calse: permission denied: %s", p.we_wordv[0]);
        else
          perror(p.we_wordv[0]);

        wordfree(&p);

        return 1;
      } else if (pid > 0) {
        wait(NULL);
        putchar('\n');
        wordfree(&p);
      } else {
        perror("fork");
        wordfree(&p);
        return 1;
      }
    }

    free(command);
  }

  return 0;
}
