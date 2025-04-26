#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int days_in_month(int month, int year) {
  if (month == 2) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
  }
  int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  return days[month - 1];
}

int main(int argc, char *argv[]) {
  int now_day, now_month, now_year;
  time_t t = time(NULL);
  struct tm *now = localtime(&t);

  now_day = now->tm_mday;
  now_month = now->tm_mon + 1;
  now_year = now->tm_year + 1900;

  int column, start_day, num_days;
  int count = (argc > 1) ? atoi(argv[1]) : 1;

  const char *MonthName[] = {"January",   "February", "March",    "April",
                             "May",       "June",     "July",     "August",
                             "September", "October",  "November", "December"};

  for (int enum_month = 0; enum_month < count; enum_month++) {
    struct tm first_day = {0};
    first_day.tm_mday = 0;
    first_day.tm_mon = now_month - 1 + enum_month;
    first_day.tm_year = now_year - 1900;
    mktime(&first_day); // normalize

    int this_month = first_day.tm_mon + 2;
    int this_year = first_day.tm_year + 1900;

    printf("     %s %d\n", MonthName[this_month - 1], now_year);
    printf("Mo Tu We Th Fr Sa Su\n");

    start_day = first_day.tm_wday;
    num_days = days_in_month(this_month, now_year);

    for (int i = 0; i < start_day; i++) {
      printf("   ");
    }

    column = start_day;
    for (int enum_day = 1; enum_day <= num_days; enum_day++) {
      if (now_day == enum_day && now_month == this_month)
        printf("\033[30;47m%2d\033[0m ", enum_day);
      else
        printf("%2d ", enum_day);
      column++;
      if (column % 7 == 0)
        printf("\n");
    }
    printf("\n\n");
  }
  return 0;
}
