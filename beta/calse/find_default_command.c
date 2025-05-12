#include "find_default_command.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * find_default_command - обработка встроенных команд оболочки
 * @commandv: массив аргументов команды (аналог argv)
 * @commandc: количество аргументов (аналог argc)
 *
 * Выполняет встроенные команды: exit, cd, logout, exec, setenv, unsetenv.
 * Возвращает:
 *   0 - команда обработана (ничего дальше не делаем)
 *   1 - команда exec, требуется завершить оболочку
 *   2 - команда exit, требуется завершить оболочку
 *  -1 - произошла ошибка (chdir, setenv, unsetenv и т.д.)
 *
 * Остальные команды не обрабатываются, возвращается -1.
 */
int find_default_command(char *commandv[], int commandc) {
  if (!strcmp(commandv[0], "exit")) {
    printf("exit\n");

    return 2;
  }

  if (!strcmp(commandv[0], "cd")) {
    const char *target = (commandc > 1) ? commandv[1] : getenv("HOME");
    if (chdir(target) == -1) {
      return -1;
    };

    return 0;
  }

  if (!strcmp(commandv[0], "logout")) {
    printf("%s: not login shell: use 'exit'\n", commandv[0]);

    return 0;
  }

  if (!strcmp(commandv[0], "exec")) {
    if (commandc > 1)
      execvp(commandv[1], &commandv[1]);

    return 1;
  }

  if (!strcmp(commandv[0], "setenv")) {
    if (commandc < 3) {
      fprintf(stderr, "Usage: export VAR VALUE\n");
      return 0;
    } else if (setenv(commandv[1], commandv[2], 1) == -1)
      return -1;

    return 0;
  }

  if (!strcmp(commandv[0], "unsetenv")) {
    if (unsetenv(commandv[1]) == -1)
      return -1;

    return 0;
  }

  return -2;
}
