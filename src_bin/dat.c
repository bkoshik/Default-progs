#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE *file;
  char buffer;

  for (int i = 1; i < argc; i++) {
    file = fopen(argv[i], "r");

    if (file == NULL) {
      perror(argv[i]);
      return 1;
    }

    long int j = 1;

    printf("\t\tFILE: %s\n\n", argv[i]);

    printf("%4ld │  ", j);

    while ((buffer = fgetc(file)) != EOF) {
      putchar(buffer);

      if (buffer == '\n') {
        j++;
        printf("%4ld │  ", j);
      }
    }

    fclose(file);
    printf("\n\n\n");
  }

  return 0;
}
