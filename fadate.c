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

int is_leap_year_geo(int year)
{
  int leap_year = 0;
  leap_year = (year % 4 == 0) && (year % 100 != 0);
  if ((year % 100) == 0)
    leap_year = (year % 400 == 0);

  return leap_year;
}

int is_leap_year_per(int year)
{
  // for year 438 to 3293, mod of 128 should be among these numbers
  int result_check[] = {
    0, 4, 8, 12, 16, 20, 24, 29, 33, 37, 41, 45, 49, 53, 57,
    62, 66, 70, 74, 78, 82, 86, 90, 95, 99, 103, 107, 111,
    115, 119, 124
  };
  // for year 1 to 473, 4 numbers are different (from wikipedia)
  int result_check_refined[] = {
    0, 4, 8, 12, 16, 20, 25, 29, 33, 37, 41, 45, 49, 53, 58,
    62, 66, 70, 74, 78, 82, 86, 91, 95, 99, 103, 107, 111,
    115, 120, 124
  };
  int leap_year = 0;
  if (year < 438)
    leap_year = is_in_array(year % 128, result_check_refined, sizeof(result_check_refined) / sizeof(result_check_refined[0]));
  else
    leap_year = is_in_array(year % 128, result_check, sizeof(result_check) / sizeof(result_check[0]));

  return leap_year;
}

int count_leap_years_geo(int year)
{
  int leap_years = 0;
  while (year)
  {
    if (is_leap_year_geo(year))
      leap_years++;
    year--;
  }

  return leap_years;
}

int count_leap_years_per(int year)
{
  int leap_years = 0;
  while (year)
  {
    if (is_leap_year_per(year))
      leap_years++;
    year--;
  }

  return leap_years;
}

int get_correct_year_geo(long days)
{
  int close_year = days / 365;
  int leap_years = count_leap_years_geo(close_year);
  int years_ahead = leap_years / 365;
  return close_year - years_ahead;
}

int get_correct_year_per(long days)
{
  int close_year = days / 365;
  int leap_years = count_leap_years_per(close_year);
  int years_ahead = leap_years / 365;
  return close_year - years_ahead;
}

int get_date_from_days_geo(int days, int *year, int *mon, int *day)
{
  int thirtyone_months[7] = {1, 3, 5, 7, 8, 10, 12};
  int thirty_months[4] = {4, 6, 9, 11};

  int correct_year_geo = get_correct_year_geo(days);
  int leap_years_geo = count_leap_years_geo(correct_year_geo);

  days -= leap_years_geo;

  *year = (int) days / 365;
  (*year)++;

  days = days % 365;

  int c = 0;
  int is_cur_year_leap_geo = is_leap_year_geo(*year);
  while (days > 28)
  {
    if (c != 2 && days < 30)
      break;

    c++;
    if (is_in_array(c, thirtyone_months, sizeof(thirtyone_months) / sizeof(thirtyone_months[0])))
    {
      if (days <= 31)
      {
        --c;
        break;
      }
      days -= 31;
    }
    else if (is_in_array(c, thirty_months, sizeof(thirtyone_months) / sizeof(thirtyone_months[0])))
    {
      if (days <= 30)
      {
        --c;
        break;
      }
      days -= 30;
    }
    else if (c == 2)
    {
      if (is_cur_year_leap_geo && days < 29)
      {
        --c;
        break;
      }

      if (!is_cur_year_leap_geo && days < 28)
      {
        --c;
        break;
      }
      days -= is_cur_year_leap_geo ? 29 : 28;
    }
  }

  *mon = ++c;
  *day = days;

  return 1;
}

int get_date_from_days_per(long days, int *year, int *mon, int *day)
{
  int correct_year_per = get_correct_year_per(days);
  int leap_years_per = count_leap_years_per(correct_year_per);

  days -= leap_years_per;

  *year = (int) days / 365;
  (*year)++;

  days = days % 365;
  printf("days: %ld\n", days);

  int is_cur_year_leap_per = is_leap_year_per(*year);

  int i = 1;
  for (; i < 13; i++)
  {
    if (days < 31)
      break;

    if (i == 12)
    {
      if (i == is_cur_year_leap_per ? 30 : 29)
      {
        i--;
        break;
      }
      days -= is_cur_year_leap_per ? 30 : 29;
    }
    else if (i > 6)
    {
      if (i == 30)
      {
        i--;
        break;
      }
      days -= 30;
    }
    else 
    {
      if (i == 31)
      {
        i--;
        break;
      }
      days -= 31;
    }
  }

  *mon = i;
  *day = days;

  return 1;
}


long count_days_since_geo(int year, int mon, int day)
{
  long days;
  int leap_years_geo;
  int is_cur_year_leap_geo;
  int thirtyone_months[7] = {1, 3, 5, 7, 8, 10, 12};
  int thirty_months[4] = {4, 6, 9, 11};

  is_cur_year_leap_geo = is_leap_year_geo(year);
  year--;
  leap_years_geo = count_leap_years_geo(year);

  days = year * 365;
  days += leap_years_geo;

  mon--;
  for (int i=1; i < mon + 1; i++)
  {
    if (is_in_array(i, thirtyone_months, sizeof(thirtyone_months) / sizeof(thirtyone_months[0])))
      days += 31;

    if (is_in_array(i, thirty_months, sizeof(thirty_months) / sizeof(thirty_months[0])))
      days += 30;

    if (i == 2)
      days += is_cur_year_leap_geo ? 29 : 28;
  }

  days += day;

  return days;
}

long count_days_since_per(int year, int mon, int day)
{
  long days;
  int leap_years_per;

  year--;
  days = year * 365;
  leap_years_per = count_leap_years_per(year);
  days += leap_years_per;

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

  return days;
}

struct tm *convert_to_georgian(int year, int mon, int day, struct tm *ltm)
{
  int gyear, gmon, gday;
  long days = count_days_since_per(year, mon, day);
  days += 226895; // difference of persian and gorgian in days
  get_date_from_days_geo(days, &gyear, &gmon, &gday);
  return make_tm(gyear, gmon, gday, ltm);
}

int convert_to_persian(int year, int mon, int day, int *pyear, int *pmon, int *pday)
{
  long days = count_days_since_geo(year, mon, day);
  days -= 226895; // difference of persian and gorgian in days
  get_date_from_days_per(days, pyear, pmon, pday);

  return 1;
}
