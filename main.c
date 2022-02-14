#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "date.h"
#include "money.h"
#include "config.h"



int main(int argc, char **argv) {
    int choice;
    int run = 1;
    int n_in_array;
    int opt;

    double *money_vec = NULL;

    char buf[BUF_SIZE];
    time_t seconds = time(NULL);
    struct tm *time_info = localtime(&seconds);

    while ((opt = getopt(argc, argv, "d:ps:")) != -1) {
        switch (opt) {
            case 'd': {
                int i = 0;
                int l = 0;
                char buf[BUF_SIZE];
                int size = BUF_SIZE/10;
                money_vec = malloc(sizeof *money_vec * size);
                while (sscanf(optarg + l, "%s", buf) == 1) {
                    l += strlen(buf) + (strstr(optarg + l, buf) - (optarg + l));
                    money_vec[i++] = atof(buf);
                    if (i > size) {
                        size *= 2;
                        money_vec = realloc(money_vec, sizeof *money_vec * size);
                    }
                }
                money_to_file(money_vec, i, time_info);
                free(money_vec);
              }
                break;

            case 'p': {
                int return_code;
                if ((return_code = money_summary_print()) != 0 ) {
                    fprintf(stdout, "Blad krytyczny, kod wyjscia %d\n", return_code);
                    return -3;
                }
            }
                break;

            case 's': {
                int month, year;
                int n = 0;
                double suma = 0;
                if (sscanf(optarg, "%d.%d", &month, &year) != 2) {
                    fprintf(stdout, "%s: -s month.year\n", argv[0]);
                    return -2;
                }
                
                if ((money_vec = money_from_file(&n, month, year)) == (double *) 2) {
                    fprintf(stdout, "Nie ma zapisanych danych\n");
                } else if (money_vec == NULL) {
                    fprintf(stdout, "Zabraklo pamieci.\n");
                    return -3;
                } else if (money_vec == (double *) 1) {
                    fprintf(stdout, "Lacznie: 0.00\n");
                } else { 
                    fprintf(stdout, "\n");
                    for (int i = 0; i < n; i++) {
                        fprintf(stdout, "%.02lf\n", money_vec[i]);
                        suma += money_vec[i];
                    }
                    fprintf(stdout, "\nLacznie: %.02lf\n", suma);
                    free(money_vec);
                }
            }
                break;
            }
    }
    
    if (argc > 1)
        return 0;

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

                if (money_report_menu(time_info) != 0) {
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
