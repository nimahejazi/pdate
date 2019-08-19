#define _XOPEN_SOURCE
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

int split_date_str(char *date, int *year, int *mon, int *day, char *splitter)
{
  char *_year = strtok(date, splitter);
  char *_mon = strtok(NULL, splitter);
  char *_day = strtok(NULL, splitter);

  if (!(_year &&  _mon && _day))
    return 1;

  *year = atoi(_year);
  *mon = atoi(_mon);
  *day = atoi(_day);

  return 0;
}

struct tm *make_tm(int year, int mon, int day, struct tm *ltm)
{
  char strtime[11];
  sprintf(strtime, "%4d-%02d-%02d", year, mon, day);
  strptime(strtime, "%Y-%m-%d", ltm);
  return ltm;
}

struct tm *make_tm_cur(struct tm *ltm)
{
  time_t cur_date;
  time(&cur_date);
  ltm = localtime(&cur_date);
  return ltm;
}
