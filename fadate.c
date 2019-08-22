#define _XOPEN_SOURCE
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int gsumdays[2][13] = {
  {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
  {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
};

int psumdays[2][13] = {
  {0, 31, 62, 93, 124, 155, 186, 216, 246, 276, 306, 336, 365},
  {0, 31, 62, 93, 124, 155, 186, 216, 246, 276, 306, 336, 366}
};

struct tm *make_tm(int year, int mon, int day, struct tm *ltm)
{
  char strtime[11];
  sprintf(strtime, "%4d-%02d-%02d", year, mon, day);
  strptime(strtime, "%Y-%m-%d", ltm);
  return ltm;
}

int gisleap(int year)
{
  return ((year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0));
}

int pisleap(int year)
{
  year = (year - 474) % 128;
  year = ((year >= 30) ? 0 : 29) + year;
  year = year - floor(year / 33) - 1;
  return ((year % 4) == 0);
}

int split_date_str(char *date, int *year, int *mon, int *day, char *splitter, char *format)
{
  char *_format[3];
  _format[0] = strtok(format, splitter);
  _format[1] = strtok(NULL, splitter);
  _format[2] = strtok(NULL, splitter);

  char *_part[3];
  _part[0] = strtok(date, splitter);
  _part[1] = strtok(NULL, splitter);
  _part[2] = strtok(NULL, splitter);


  if (!(*_part[0] && *_part[1] && *_part[2]))
    return 1;

  for (int i=0; i < 3; i++)
  {
    if (strcmp(_format[i], "YYYY") == 0 || strcmp(_format[i], "YY") == 0)
        *year = atoi(_part[i]);
    else if (strcmp(_format[i], "MM") == 0 || strcmp(_format[i], "M") == 0)
        *mon = atoi(_part[i]);
    else if (strcmp(_format[i], "DD") == 0 || strcmp(_format[i], "D") == 0)
        *day = atoi(_part[i]);
  }

  return 0;
}

int to_shamsi(int gyear, int gmon, int gday, int *pyear, int *pmon, int *pday)
{
  *pyear = gyear - 621;

  int gleap = gisleap(gyear);
  int pleap = pisleap(*pyear - 1);

  int pelapsed;
  int gelapsed = gsumdays[(gleap ? 1 : 0)][gmon -1] + gday;

  int xmastonorooz = (pleap && gleap) ? 80 : 79;

  if (gelapsed <= xmastonorooz)
  {
    pelapsed = gelapsed + 286;
    (*pyear)--;
    if (pleap && !gleap)
      pelapsed++;
  }
  else
  {
    pelapsed = gelapsed - xmastonorooz;
    pleap = pisleap(*pyear);
  }

  for (int i = 1; i <= 12; i++)
  {
    if (psumdays[(pleap ? 1 : 0)][i] >= pelapsed)
    {
      *pmon = i;
      *pday = pelapsed - psumdays[(pleap ? 1 : 0)][i - 1];
      break;
    }
  }

  return 1;
}

int to_georgian(int pyear, int pmon, int pday, struct tm *tml)
{
  int gyear, gmon, gday;
  gyear = pyear + 621;

  int pleap = pisleap(pyear);
  int gleap = gisleap(gyear);

  int pelapsed = psumdays[pleap ? 1 : 0][pmon - 1] + pday;
  int gelapsed;

  if (pmon > 10 || (pmon == 10 && pelapsed > 286 + (gleap ? 1 : 0)))
  {
    gelapsed = pelapsed - (286 + (gleap ? 1 : 0));
    gleap = gisleap(++gyear);
  }
  else
  {
    pleap = pisleap(pyear - 1);
    gelapsed = pelapsed + 79 + (pleap ? 1 : 0) - (gisleap(gyear - 1) ? 1 : 0);
  }

  for (int i = 1; i <= 12; i++)
  {
    if (gsumdays[gleap ? 1 : 0][i] >= gelapsed)
    {
      gmon = i;
      gday = gelapsed - gsumdays[gleap ? 1 : 0][i - 1];
      break;
    }
  }

  make_tm(gyear, gmon, gday, tml);

  return 1;
}

