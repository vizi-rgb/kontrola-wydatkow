#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "money.h"
#include "config.h"
#include "date.h"

double *money_stdin(int *i) {
    int size = 100;
    double *money = malloc(sizeof *money * size);
    char character; 

    while (scanf("%lf%c", money + (*i)++, &character) == 2) {
        if (*i == size - 2) {
            size *= 2;
            money = realloc(money, sizeof *money * size);
        }

        if (character == '\n')
            break;
    }

    money = realloc(money, sizeof *money * *i);
    return money;
}


double *money_from_file(int *i, struct tm *time_info) {
    FILE *in = fopen(FILE_NAME, "r");
    if (in == NULL)
        return NULL;

    int size = 100;
    double *money = malloc(sizeof *money * size);

    if (fseek(in, date_find(time_info), SEEK_SET) != 0)
        return NULL;

    while (fscanf(in, "%lf\n", money + *i) == 1) {
        if (*i == size - 2) {
            size *= 2;
            money = realloc(money, sizeof *money * size);
        }
        (*i)++;
    }

    money = realloc(money, sizeof *money * *i);

    return money;

}

void money_to_file(double *money_vec, int n, struct tm *time_info) {
    FILE *out = fopen(FILE_NAME, "a");
    if (!date_check(time_info)) {
        fprintf(out, "-- %02d.%d\n", time_info->tm_mon, time_info->tm_year + 1900); 
    }

    for (int i = 0; i < n; i++)  
        fprintf(out, "%.2lf\n", fabs(money_vec[i]));

    fclose(out);

}
