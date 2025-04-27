#include <cjson/cJSON.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEMPLATES 128

void replace_backslash_with_percent(char *str);
const char *find_template(const char *key);
void load_templates_json();

typedef struct {
  char key[64];
  char format[256];
} Template;

Template templates[MAX_TEMPLATES];
ssize_t templates_count = 0;

int main(int argc, char *argv[]) {
  int opt;
  time_t t = time(NULL);
  struct tm *now = localtime(&t);
  char buffer[256];
  char str_time[256];

  load_templates_json();

  while ((opt = getopt(argc, argv, "t:")) != -1) {
    switch (opt) {
    case 't':
      const char *preset = find_template(optarg);
      snprintf(str_time, sizeof(str_time), "%s", preset ? preset : optarg);
    }
  }

  replace_backslash_with_percent(str_time);

  if (strftime(buffer, sizeof(buffer), str_time, now) == 0)
    fprintf(stderr, "Error: Formating time\n");

  printf("%s\n", buffer);

  return 0;
}

void replace_backslash_with_percent(char *str) {
  for (int i = 0; str[i]; i++) {
    if (str[i] == '\\') {
      if (str[i + 1] == 'n') {
        str[i] = '\n';
        memmove(&str[i + 1], &str[i + 2], strlen(&str[i + 2]) + 1);
      } else if (str[i + 1] == 't') {
        str[i] = '\t';
        memmove(&str[i + 1], &str[i + 2], strlen(&str[i + 2]) + 1);
      } else if (str[i + 1] == '\\') {
        str[i] = '\\';
        memmove(&str[i + 1], &str[i + 2], strlen(&str[i + 2]) + 1);
      } else if (str[i + 1] == '%') {
        str[i] = '%';
        memmove(&str[i + 1], &str[i + 2], strlen(&str[i + 2]) + 1);
      } else {
        i--;
      }
    }
  }
}

const char *find_template(const char *key) {
  for (size_t i = 0; i < templates_count; i++) {
    if (strcmp(templates[i].key, key) == 0)
      return templates[i].format;
  }
  return NULL;
}

void load_templates_json() {
  char *home = getenv("HOME");
  if (!home)
    return;

  char path[256];
  snprintf(path, sizeof(path), "%s/.config/datetime_presets.json", home);

  FILE *f = fopen(path, "r");
  if (!f)
    return;

  // Читаем весь файл в память
  fseek(f, 0, SEEK_END);
  long file_size = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *json_data = (char *)malloc(file_size + 1);
  fread(json_data, 1, file_size, f);
  json_data[file_size] = '\0'; // NULL-terminate

  fclose(f);

  // Парсим JSON
  cJSON *json = cJSON_Parse(json_data);
  if (json == NULL) {
    fprintf(stderr, "Error parsing JSON\n");
    free(json_data);
    return;
  }

  // Обрабатываем каждый шаблон
  cJSON *item = NULL;
  cJSON_ArrayForEach(item, json) {
    cJSON *key = cJSON_GetObjectItemCaseSensitive(item, "key");
    cJSON *format = cJSON_GetObjectItemCaseSensitive(item, "format");

    if (cJSON_IsString(key) && cJSON_IsString(format)) {
      if (templates_count < MAX_TEMPLATES) {
        strncpy(templates[templates_count].key, key->valuestring,
                sizeof(templates[templates_count].key) - 1);
        strncpy(templates[templates_count].format, format->valuestring,
                sizeof(templates[templates_count].format) - 1);
        templates_count++;
      }
    }
  }

  cJSON_Delete(json);
  free(json_data);
}
