#ifndef MONEY_H
#define MONEY_H

#include <time.h>

double *money_stdin(int *);
double *money_from_file(int *, int, int);
void money_to_file(double *, int, struct tm *);
double *money_summary(int *);

#endif
