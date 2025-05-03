#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 128
#define MAX_FIELD 256

int main(void) {
  FILE *fp = fopen("/proc/mounts", "r");
  if (!fp) {
    perror("fopen");
    return 1;
  }

  char line[1024];

  char devs[MAX_LINES][MAX_FIELD];
  char mounts[MAX_LINES][MAX_FIELD];
  char fss[MAX_LINES][MAX_FIELD];

  int count = 0;
  size_t max_dev = 0, max_mount = 0;

  while (fgets(line, sizeof(line), fp)) {
    if (strncmp(line, "/dev/", 5) == 0) {
      char *token = strtok(line, " ");
      int field = 0;

      while (token) {
        field++;
        if (field == 1)
          strncpy(devs[count], token, MAX_FIELD);
        else if (field == 2)
          strncpy(mounts[count], token, MAX_FIELD);
        else if (field == 3) {
          strncpy(fss[count], token, MAX_FIELD);
          break;
        }

        token = strtok(NULL, " ");
      }

      size_t dev_len = strlen(devs[count]);
      size_t mount_len = strlen(mounts[count]);

      if (dev_len > max_dev)
        max_dev = dev_len;
      if (mount_len > max_mount)
        max_mount = mount_len;

      count++;
      if (count >= MAX_LINES)
        break;
    }
  }

  fclose(fp);

  printf("\033[1mNAME             MOUNTPOINT  FSTYPE\033[0m\n");
  for (int i = 0; i < count; i++) {
    printf("%-*s   %-*s   %s\n", (int)max_dev, devs[i], (int)max_mount,
           mounts[i], fss[i]);
  }

  return 0;
}
