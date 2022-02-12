#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "date.h"
#include "money.h"
#include "config.h"

int main(int argc, char **argv) {
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
                n_of_inputs = 0;
                money_vec = money_stdin(&n_of_inputs); 
                money_to_file(money_vec, n_of_inputs, time_info);
                free(money_vec);
            }
            break;
            
            case 'R':
            case 'r': {
                n_of_dates_in_file = 0;
                date_vec = date_list(&n_of_dates_in_file);
                for (int i = 0; i  < n_of_dates_in_file; i += 2) {
                    printf("miech %d rok %d\n", date_vec[i], date_vec[i+1]);
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
