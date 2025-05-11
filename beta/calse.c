#include <netdb.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wordexp.h>

#define MAXNAMEUSER 1024
#define MAXPROMPT 2048
#define MAXCOMMAND 4096

int getprompt(char prompt[]);

int main(void) {
  signal(SIGINT, SIG_IGN);

  while (1) {
    char prompt[MAXPROMPT] = {0};

    if (getprompt(prompt) == -1) {
      perror("getprompt");
      return 1;
    }

    char command[MAXCOMMAND];

    printf("%s", prompt);
    fflush(stdout);

    fgets(command, MAXCOMMAND, stdin);
    command[strcspn(command, "\n")] = 0;

    if (strlen(command) == 0)
      continue;

    wordexp_t p;
    wordexp(command, &p, 0);

    if (!strcmp(p.we_wordv[0], "exit")) {
      printf("exit\n");
      wordfree(&p);
      break;
    }

    if (!strcmp(p.we_wordv[0], "cd")) {
      const char *target = (p.we_wordc > 1) ? p.we_wordv[1] : getenv("HOME");
      if (chdir(target) == -1) {
        perror("cd");
      };
      wordfree(&p);
      continue;
    }

    if (!strcmp(p.we_wordv[0], "logout")) {
      printf("%s: not login shell: use 'exit'\n", p.we_wordv[0]);

      wordfree(&p);
      continue;
    }

    if (!strcmp(p.we_wordv[0], "exec")) {
      if (p.we_wordc > 1)
        execvp(p.we_wordv[1], &p.we_wordv[1]);

      wordfree(&p);
      break;
    }

    pid_t pid = fork();

    if (pid == 0) {
      signal(SIGINT, SIG_DFL);
      execvp(p.we_wordv[0], p.we_wordv);

      perror(p.we_wordv[0]);
      wordfree(&p);

      return 1;
    } else if (pid > 0) {
      wait(NULL);
      putchar('\n');
    } else {
      perror("fork");
      return 1;
    }
  }

  return 0;
}

int getprompt(char prompt[]) {
  char host[MAXNAMEUSER];
  struct hostent *h;

  /* get host's name (user@HOST <- this) */
  gethostname(host, sizeof(host));
  host[sizeof(host) - 1] = '\0';

  /* get user's name (this -> USER@host) */
  uid_t uid = getuid();
  struct passwd *pw = getpwuid(uid);
  if (!pw) {
    perror("getpwuid");
    return -1;
  }

  const char *user = pw->pw_name;
  char user_root;

  /* get absolute path */
  char *home = getenv("HOME");
  char *cwd = getcwd(NULL, 0);
  if (cwd == NULL) {
    perror("getcwd");
    return 1;
  }

  /* If root prompt will be #
     else if user prompt will be $ */
  if (uid != 0)
    user_root = '$';
  else
    user_root = '#';

  /* /home/user -> ~
   * /home/other_user -> /home/other_user  */
  char *fcwd;
  char fcwd_buf[MAXNAMEUSER];

  if (cwd && home && strncmp(cwd, home, strlen(home)) == 0) {
    snprintf(fcwd_buf, sizeof(fcwd_buf), "~%s", cwd + strlen(home));
    fcwd = fcwd_buf;
  } else
    fcwd = cwd;
  snprintf(prompt, MAXPROMPT, "[%s@%s (%s)]%c> ", user, host, fcwd, user_root);

  free(cwd);

  return 0;
}
