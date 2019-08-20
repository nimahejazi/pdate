int split_date_str(char *date, int *year, int *mon, int *day, char *splitter);
struct tm *make_tm(int year, int mon, int day, struct tm *ltm);
struct tm *make_tm_cur(struct tm *ltm);
struct tm *convert_to_georgian(int year, int mon, int day, struct tm *ltm);
int convert_to_persian(int year, int mon, int day, int *pyear, int *pmon, int *pday);
