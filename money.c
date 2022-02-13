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


double *money_from_file(int *i, int m, int y) {
    FILE *in = fopen(FILE_NAME, "r");
    if (in == NULL)
        return NULL;

    int size = 100;
    double *money = malloc(sizeof *money * size);

    if (date_find(m, y) < 0 || fseek(in, date_find(m, y), SEEK_SET) != 0) {
        fclose(in);
        free(money);
        return NULL;
    }

    while (fscanf(in, "%lf\n", money + *i) == 1) {
        if (*i == size - 2) {
            size *= 2;
            money = realloc(money, sizeof *money * size);
        }
        (*i)++;
    }

    money = realloc(money, sizeof *money * *i);

    fclose(in);

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

double *money_summary(int *n) {
    *n = 0;
    int *date_vec = date_list(n);
    if (date_vec == NULL) {
        return NULL;
    }

    double *money = NULL;
    int n_of_sums;
    double *money_per_month = calloc((*n + 1)/2, sizeof *money_per_month);

    if (money_per_month == NULL) {
        return NULL;
    }

    for (int i = 0; i < (*n + 1)/2; i++) {
        n_of_sums = 0;
        if ((money = money_from_file(&n_of_sums, date_vec[2 * i], date_vec[2 * i + 1])) == NULL && n_of_sums != 0) {
            return NULL;
        }

        for (int j = 0; j < n_of_sums; j++) {
            money_per_month[i] += money[j];
        }
        free(money);
    }

    *n = (*n + 1)/2;
    free(date_vec);
    return money_per_month;
}
