#ifndef DATE_H
#define DATE_H

#include <time.h>

int date_check(struct tm *);
long int date_find(int, int);
int *date_list(int *);

#endif
