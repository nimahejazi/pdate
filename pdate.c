#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fadate.h"

int main(int argc, char **argv)
{
  char *date_to_convert;
  /* default arguments */
  int to_georgian = 0;


  struct tm *tml = malloc(sizeof(struct tm));

  char *app_name = malloc(strlen(*argv) + 1);
  strcpy(app_name, *argv);

  while(argv++, --argc)
  {
    if (!strcmp(*argv, "-g") || !strcmp(*argv, "--to-georgian"))
      to_georgian = 1;
    
    if (argc == 1)
    {
      date_to_convert = malloc(strlen(*argv) + 1);
      strcpy(date_to_convert, *argv);
    }
  }

  int year, mon, day;
  int pyear, pmon, pday;

  char *format = malloc(strlen("YYYY/MM/DD") + 1);
  strcpy(format, "YYYY/MM/DD");
  split_date_str(date_to_convert, &year, &mon, &day, "/", format);

  if (to_georgian)
  {
    convert_to_georgian(year, mon, day, tml);
    printf("%4d/%02d/%02d\n", tml->tm_year + 1900, tml->tm_mon, tml->tm_mday);
  }
  else 
  {
    convert_to_persian(year, mon, day, &pyear, &pmon, &pday);
    printf("%4d/%02d/%02d\n", pyear, pmon, pday);
  }

  
  free(app_name);
  free(date_to_convert);
  free(tml);
}
