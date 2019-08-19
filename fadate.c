#include <stdlib.h>
#include <string.h>

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
