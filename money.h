#ifndef MONEY_H
#define MONEY_H

#include <time.h>

double *money_stdin(int *);
double *money_from_file(int *, struct tm *);
void money_to_file(double *, int, struct tm *);

#endif
