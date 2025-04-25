#include <fcntl.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#define BOLD "\033[1m"
#define RESET "\033[0m"

ssize_t read_file(const char *path, char *buffer, size_t size);
void print_label_value(const char *label, const char *value);

int main(int argc, char *argv[]) {
  char buffer[16];

  read_file("/sys/class/power_supply/BAT0/capacity", buffer, sizeof(buffer));
  print_label_value("Capacity: ", buffer);

  read_file("/sys/class/power_supply/BAT0/capacity_level", buffer,
            sizeof(buffer));
  print_label_value("Level: ", buffer);

  read_file("/sys/class/power_supply/AC0/online", buffer, sizeof(buffer));
  print_label_value("AC Status: ",
                    buffer[0] == '1' ? "Connected" : "AC Disconnected");

  return 0;
}

ssize_t read_file(const char *path, char *buffer, size_t size) {
  int fd = open(path, O_RDONLY);
  if (fd == -1)
    return -1;

  ssize_t bytes = read(fd, buffer, size - 1);
  close(fd);
  if (bytes >= 0)
    buffer[bytes] = '\0';
  return bytes;
}

void print_label_value(const char *label, const char *value) {
  write(STDOUT_FILENO, BOLD, sizeof(BOLD));
  write(STDOUT_FILENO, label, strlen(label));
  write(STDOUT_FILENO, RESET, sizeof(RESET));
  write(STDOUT_FILENO, value, strlen(value));
  if (value[strlen(value) - 1] != '\n')
    write(STDOUT_FILENO, "\n", 1);
}
