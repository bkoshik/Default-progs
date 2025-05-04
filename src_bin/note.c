#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NOTES_FILE "/home/[user]/notes.txt"
#define COUNTER_FILE "/home/[user]/.config/counter.txt"
#define MAX_NOTE 2048

int int_to_str(int num, char *buf);

int main(void) {
  /* Reading and writing count of notes */
  int fd = open(COUNTER_FILE, O_RDWR | O_CREAT, 0600); // Opening counter file

  char buf_counter[16] = {0}; // Buffer for count
  ssize_t bytes =
      read(fd, buf_counter, sizeof(buf_counter) - 1); // Reading counter file
  int count = 1;

  if (bytes > 0) {
    count = atoi(buf_counter); // str -> int
    if (count <= 0)
      count = 1;
  }

  count++;
  int len = int_to_str(count, buf_counter); // int -> str (for below functions)

  lseek(fd, 0, SEEK_SET);      // moving to start
  write(fd, buf_counter, len); // Writing new count
  ftruncate(fd, len);          // Cutting file for lenght ($len)
  close(fd);                   // Closing

  /* Writing note */
  fd = open(NOTES_FILE, O_WRONLY | O_CREAT | O_APPEND,
            0600); // Opening notes file
  if (fd == -1) {
    perror("note");
    return 1;
  }

  char buffer[MAX_NOTE] = {0}; // Creating buffer for notes
  int i = strlen(buf_counter) +
          1; // i's lenght is strlen(...) + 1, because of below for cycle
  char c;    // Buffer for reading per 1 c(har)

  for (int j = 0; buf_counter[j] != '\0';
       j++) // This for cycle using for (xD) writing count of note
    buffer[j] = buf_counter[j]; // Writing count
  buffer[i++] = '.';            // for "1."
  buffer[i++] = ' ';            // for "1. "

  while (read(STDIN_FILENO, &c, 1) > 0 &&
         c != '\n')   // Reading from stdin and writing to c(har) per 1 char
    buffer[i++] = c;  // Writing c(har) to buffer
  buffer[i++] = '\n'; // End the note with a newline

  write(fd, buffer, i); // Writing note from buffer to notes.txt

  close(fd); // Closing

  return 0;
}

int int_to_str(int num, char *buf) {
  int len = 0;

  if (num == 0) {
    buf[len++] = '0';
  }

  int tmp = num;
  while (tmp > 0) {
    buf[len++] = '0' + (tmp % 10);
    tmp /= 10;
  }

  buf[len] = '\0';

  for (int i = 0; i < len / 2; i++) {
    char temp = buf[i];
    buf[i] = buf[len - i - 1];
    buf[len - i - 1] = temp;
  }

  return len;
}
