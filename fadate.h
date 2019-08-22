int to_shamsi(int gyear, int gmon, int gday, int *pyear, int *pmon, int *pday);
int to_georgian(int pyear, int pmon, int pday, struct tm *tml);
int split_date_str(char *date, int *year, int *mon, int *day, char *splitter, char *format);
