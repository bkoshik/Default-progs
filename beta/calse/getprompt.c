#include "getprompt.h"
#include <netdb.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * getprompt - формирует строку приглашения вида [user@host (cwd)]$>
 * @prompt: указатель на буфер, куда будет записана строка приглашения
 * @max_prompt_len: максимальная длина для приглашения
 * @max_nameuser_len: максимальная длина имени пользователя и названия
 * компьютера, формата user@host
 *
 * Формирует строку приглашения вида "[user@host (cwd)]$> " или "#>" для root.
 * Показывает текущего пользователя, хост и рабочий каталог.
 * Упрощает /home/user до ~.
 *
 * Return: 0 при успехе, -1 при ошибке (например, getpwuid или getcwd
 * сломались).
 */
int getprompt(char prompt[], int max_prompt_len, int max_nameuser_len) {
  char host[max_nameuser_len];

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
  char fcwd_buf[max_nameuser_len];

  if (cwd && home && strncmp(cwd, home, strlen(home)) == 0) {
    snprintf(fcwd_buf, sizeof(fcwd_buf), "~%s", cwd + strlen(home));
    fcwd = fcwd_buf;
  } else
    fcwd = cwd;
  snprintf(prompt, max_prompt_len, "[%s@%s (%s)]%c> ", user, host, fcwd,
           user_root);

  free(cwd);

  return 0;
}
