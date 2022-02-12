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
    char buf[BUF_SIZE];
    double *money_vec = NULL;

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
                printf("date_find = %ld\n", date_find(time_info));
                n_of_sums_in_file = 0;
                money_vec = money_from_file(&n_of_sums_in_file, time_info);
                for (int i = 0; i < n_of_sums_in_file; i++) {
                    printf("%lf\n", money_vec[i]);
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
