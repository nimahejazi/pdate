#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fadate.h"

int main(int argc, char **argv)
{
  int year, mon, day;
  while (1)
  {
    printf("\n----------------\n");
    printf("Enter date: YYYY MM DD\n");
    scanf("%d %d %d", &year, &mon, &day);
    struct tm *tml = malloc(sizeof(struct tm));
    convert_to_georgian(year, mon, day, tml);
    free(tml);
  }
}
