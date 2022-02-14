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

    while (scanf("%lf%c", money + *i, &character) == 2) {
        if (*i == size - 1) {
            size *= 2;
            money = realloc(money, sizeof *money * size);
        }

        (*i)++;

        if (character == '\n')
            break;
    }

    money = realloc(money, sizeof *money * *i);
    return money;
}


double *money_from_file(int *i, int m, int y) {
    FILE *in = fopen(FILE_NAME, "r");
    if (in == NULL)
        return (double *) 2;

    int size = 100;
    double *money = malloc(sizeof *money * size);

    if (money == NULL) {
        fclose(in);
        return NULL;
    }

    if (date_find(m, y) < 0 || fseek(in, date_find(m, y), SEEK_SET) != 0) {
        fclose(in);
        free(money);
        return (double *) 1;
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

int money_summary_print() { 
            int n = 0;
            int *date_vec = NULL;
            double *money_vec = NULL;

            if ((date_vec = date_list(&n)) == (int *)3) {
                return -1;
            } else if (date_vec == NULL) {
                fprintf(stdout, "W bazie nie ma jeszcze rekordow!\n");
                return 0;
            }

            if ((money_vec = money_summary(&n)) == NULL) {
                free(date_vec);
                return -2;
            }

            fprintf(stdout, "\n");
            for (int i = 0; i < n; i++) {
                fprintf(stdout, "\t%02d.%d | %.2lf \n", date_vec[2 * i], date_vec[2 * i + 1], money_vec[i]);
            }
            fprintf(stdout, "\n");

            free(date_vec);
            free(money_vec); 

            return 0;
}

int money_report_menu(struct tm *time_info) {
    char buf[BUF_SIZE];
    int choice, n;

    int *date_vec = NULL;
    double *money_vec = NULL;

    fgets(buf, BUF_SIZE, stdin);

    switch(buf[0]) {
        case 'P':
        case 'p': {
            if (money_summary_print() != 0)
                return -3;
        }
        break;

        case 'S':
        case 's': {
            fprintf(stdout, "\nWybierz miesiac:\n"); 
            n = 0; // elements in array date_vec
            if ((date_vec = date_list(&n)) == NULL) {
                fprintf(stdout, "W bazie nie ma jeszcze rekordow!\n");
                return 0;
            } else if (date_vec == (int *) 3) {
                return -3;
            }

            for (int i = 0; i < n; i += 2) {
                fprintf(stdout, "%d. %02d.%d\n", i/2 + 1, date_vec[i], date_vec[i+1]);
            }
            fprintf(stdout, "0. Aktualny miesiac\n");

            do {
                fgets(buf, BUF_SIZE, stdin);
                if (sscanf(buf, "%d", &choice) < 1) 
                    return -1;

            } while (choice < 0 || choice > n/2);

            n = 0; // elements in money_vec

            if (choice == 0) {
                money_vec = money_from_file(&n, time_info->tm_mon, time_info->tm_year + 1900);
            } else {
                money_vec = money_from_file(&n, date_vec[(choice - 1) * 2], date_vec[2 * choice - 1]);
            }

            if (n != 0 && money_vec == NULL) {
                free(date_vec);
                fprintf(stdout, "Zabraklo pamieci...\n");
                return -3;
            }

            double suma = 0;
            fprintf(stdout, "\n");
            for (int i = 0; i < n; i++) {
                fprintf(stdout, "%.2lf\n", money_vec[i]);
                suma += money_vec[i];
            }
            fprintf(stdout, "---\n");
            fprintf(stdout, "Lacznie: %.2lf\n\n", suma); 

            free(money_vec);
            free(date_vec);

        }
        break;

        default: break;
    }

    return 0; 
}
