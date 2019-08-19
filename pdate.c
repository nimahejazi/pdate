#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fadate.h"

int main(int argc, char **argv)
{
  char *date = malloc(strlen("2018/10/22") + 1);
  strcpy(date, "2018/10/22");
  int year, mon, day;
  split_date_str(date, &year, &mon, &day, "/");
  printf("Year: \t%d\n", year);
  printf("Month: \t%d\n", mon);
  printf("Day: \t%d\n", day);

  free(date);
  return 0;
}
