#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "date.h"
#include "money.h"
#include "config.h"

// TODO testy
// TODO edycja zapisanych rekordow

int report_menu(struct tm *time_info) {
    char buf[BUF_SIZE];
    int choice, n;

    int *date_vec = NULL;
    double *money_vec = NULL;

    fgets(buf, BUF_SIZE, stdin);

    switch(buf[0]) {
        case 'P':
        case 'p': {
            n = 0;
            if ((date_vec = date_list(&n)) == NULL) {
                return -1;
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
        }
        break;

        case 'S':
        case 's': {
            fprintf(stdout, "\nWybierz miesiac:\n"); 
            n = 0; // elements in array date_vec
            if ((date_vec = date_list(&n)) == NULL) {
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

            } while (choice < 0 || choice > n/2 + 1);

            n = 0; // elements in money_vec

            if (choice == 0) {
                money_vec = money_from_file(&n, time_info->tm_mon, time_info->tm_year + 1900);
            } else {
                money_vec = money_from_file(&n, date_vec[(choice - 1) * 2], date_vec[2 * choice - 1]);
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

int main(int argc, char **argv) {
    int choice;
    int run = 1;
    int n_in_array;

    double *money_vec = NULL;

    char buf[BUF_SIZE];
    time_t seconds = time(NULL);
    struct tm *time_info = localtime(&seconds);

    while (run) {
        fprintf(stdout,  "\t[D]odaj kwote\n"
                         "\t[R]aport\n"
                         "\t[W]yjdz\n");

        fgets(buf, BUF_SIZE, stdin);
        choice = buf[0];

        switch(choice) {
            case 'D':
            case 'd': {
                fprintf(stdout, "Kwoty:\n");
                n_in_array = 0;
                money_vec = money_stdin(&n_in_array); 
                money_to_file(money_vec, n_in_array, time_info);
                free(money_vec);
            }
            break;
            
            case 'R':
            case 'r': {
                fprintf(stdout, "\t[P]odsumowanie\n"
                                "\t[S]zczegolowo\n"
                                "\t[W]stecz\n");

                if (report_menu(time_info) != 0) {
                    fprintf(stderr, "Blad [case 'R']\n");
                    return -1; 
                }

            }
            break;

            case 'W':
            case 'w': {
                printf("Wychodzenie z programu...\n");
                run = 0;
            }
            break;

            default: break;
        }
    }

    return 0;
}
