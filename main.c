#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "date.h"
#include "money.h"
#include "config.h"

int report_menu(int *date_vec, int *n_of_d, double *money_vec, int *n_of_s, struct tm *time_info) {
    char buf[BUF_SIZE];
    int choice;
    fgets(buf, BUF_SIZE, stdin);

    switch(buf[0]) {
        case 'P':
        case 'p': {
            ; //podsumowanie
        }
        break;

        case 'S':
        case 's': {
            *n_of_d = 0;
            fprintf(stdout, "Wybierz miesiac:\n"); 
            date_vec = date_list(n_of_d);
            for (int i = 0; i < *n_of_d; i += 2) {
                fprintf(stdout, "%d. %02d.%d\n", i/2 + 1, date_vec[i], date_vec[i+1]);
            }
            fprintf(stdout, "0. Aktualny miesiac\n");

            do {
                fgets(buf, BUF_SIZE, stdin);
                if (sscanf(buf, "%d", &choice) < 1) 
                    return -1;

            } while (choice < 0 || choice > *n_of_d/2 + 1);

            *n_of_s = 0;

            if (choice == 0) {
                money_vec = money_from_file(n_of_s, time_info->tm_mon, time_info->tm_year + 1900);
            } else {
                money_vec = money_from_file(n_of_s, date_vec[(choice - 1) * 2], date_vec[2 * choice - 1]);
            }

            double suma = 0;
            for (int i = 0; i < *n_of_s; i++) {
                fprintf(stdout, "%.2lf\n", money_vec[i]);
                suma += money_vec[i];
            }
            fprintf(stdout, "---\n");
            fprintf(stdout, "Lacznie: %.2lf\n", suma); 

        }
        break;

        default: break;
    }

    return 0; 
}

int main(int argc, char **argv) {
    // TODO zrobic porzadek ze zmiennymi n_of...
    int choice;
    int run = 1;
    int n_of_inputs;
    int n_of_sums_in_file;
    int n_of_dates_in_file;

    double *money_vec = NULL;
    int *date_vec = NULL;

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
                n_of_inputs = 0;
                money_vec = money_stdin(&n_of_inputs); 
                money_to_file(money_vec, n_of_inputs, time_info);
                free(money_vec);
            }
            break;
            
            case 'R':
            case 'r': {
                fprintf(stdout, "\t[P]odsumowanie\n"
                                "\t[S]zczegolowo\n"
                                "\t[W]stecz\n");

                if (report_menu(date_vec, &n_of_dates_in_file, money_vec, &n_of_sums_in_file, time_info) != 0) {
                    fprintf(stderr, "Blad [case 'R']\n");
                    return -1; 
                }

                free(date_vec);
                free(money_vec);
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
