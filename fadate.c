#define _XOPEN_SOURCE
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

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

int is_in_array(int no, int *arr, int size)
{
  for (int i=0; i < size; i++)
  {
    if (arr[i] == no)
      return 1;
  }

  return 0;
}

/* 
** with a number of days, we don't know how many years are
** leap years. Because simply divide it to 365 give a year
** with all leap years (days) to it. For converting it to
** a very distance future, it might add 1 day more.
** With following formula we remove 
*/
int count_leap_years(int days)
{
  int estimate_year = (int) days / 365;
  int years_ahead = (int) estimate_year / 4 / 365;
  int correct_year = estimate_year - years_ahead;
  correct_year -= 1; // current year doesn't count
  return (int) floor(correct_year / 4);
}

struct tm *convert_to_georgian(int year, int mon, int day, struct tm *ltm)
{
  int gyear, gmon, gday;
  long days;
  int leap_years_georgian;
  int leap_years;
  int thirtyone_months[7] = {1, 3, 5, 7, 8, 10, 12};
  int thirty_months[4] = {4, 6, 9, 11};

  year--;
  days = year * 365;
  leap_years = floor((year + 1) / 4);
  printf("leap_years persian: %d\n", leap_years);
  days += leap_years;

  mon--;
  if (mon > 6)
  {
    days += (mon - (mon % 6)) * 31;
    days += (mon % 6) * 30;
  }
  else
  {
    days += mon * 31;
  }

  days += day;
  days += 226899; // difference of persian and gorgian in days

  printf("%ld\n", days);

  leap_years_georgian = count_leap_years(days);

  days -= leap_years_georgian;

  gyear = (int) days / 365;
  gyear++;

  days = days % 365;

  printf("leap years georgian: %d\n", leap_years_georgian);
  printf("days left: %ld\n", days);

  
  int is_cur_gyear_leap = 0;
  int is_last_gyear_leap = 0;
  if ((gyear % 4) == 0)
    is_cur_gyear_leap = 1;

  if (((gyear - 1) % 4) == 0)
    is_last_gyear_leap = 1;

  printf("is_cur_gyear_leap: %d\n", is_cur_gyear_leap);
  printf("is_last_gyear_leap: %d\n", is_last_gyear_leap);

  int c = 0;
  while (days > 28)
  {
    c++;
    if (is_in_array(c, thirtyone_months, sizeof(thirtyone_months) / sizeof(thirtyone_months[0])))
    {
      printf("month %d is in array 31 days\n", c);
      days -= 31;
    }
    else if (is_in_array(c, thirty_months, sizeof(thirtyone_months) / sizeof(thirtyone_months[0])))
    {
      printf("month %d is in array 30 days\n", c);
      days -= 30;
    }
    else if (c == 2)
    {
      printf("month %d is %d days\n", c, is_cur_gyear_leap ? 29 : 28);
      days -= is_cur_gyear_leap ? 29 : 28;
    }
  }

  gmon = ++c;
  gday = days;

  printf("The equivalent georgian date: %4d/%02d/%02d\n", gyear, gmon, gday);
  
}
