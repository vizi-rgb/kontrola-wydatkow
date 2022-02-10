#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define STDIN_SIZE 100

#ifndef FILE_NAME
    #define FILE_NAME "data"
#endif

int date_check() {
    FILE *we = fopen(FILE_NAME, "r");

    if (we == NULL)
        return 0;

    time_t seconds = time(NULL);
    struct tm *time_info = localtime(&seconds);
    char buf[STDIN_SIZE];
    int month, year;


    while (fgets(buf, STDIN_SIZE, we) != NULL) {
        if (buf[0] == '-') {
            sscanf(buf + 3, "%d.%d", &month, &year);
            if (month == time_info->tm_mon && year == time_info->tm_year + 1900) {
                fclose(we);
                return 1;
            }
        }
    }

    fclose(we);
    return 0;
}

double *money_stdin(int *i) {
    int size = 100;
    double *money = malloc(sizeof *money * size);
    char character; 

    printf("money_stdin()\n");
    while (scanf("%lf%c", money + (*i)++, &character) == 2) {
        if (*i == size - 2) {
            size *= 2;
            money = realloc(money, sizeof *money * size);
        }

        if (character == '\n')
            break;
    }

    money = realloc(money, sizeof *money * *i);
    date_check();
    return money;
}

void money_to_file(double *money_vec, int n) {
    FILE *out = fopen(FILE_NAME, "a");
    time_t seconds = time(NULL);
    struct tm *local_time = localtime(&seconds);
    if (!date_check()) {
        fprintf(out, "-- %02d.%d\n", local_time->tm_mon, local_time->tm_year + 1900); 
    }

    for (int i = 0; i < n; i++)  
        fprintf(out, "%.2lf\n", money_vec[i]);

    fclose(out);

}


int main(int argc, char **argv) {
    FILE *file;
    int choice;
    int run = 1;
    int n_of_inputs;
    char buf[STDIN_SIZE];
    double *money_vec;

    while (run) {
        fprintf(stdout,  "\t[D]odaj kwote\n"
                         "\t[R]aport\n"
                         "\t[W]yjdz\n");

        fgets(buf, STDIN_SIZE, stdin);
        choice = buf[0];

        switch(choice) {
            case 'D':
            case 'd': {
                n_of_inputs = 0;
                money_vec = money_stdin(&n_of_inputs); 
                money_to_file(money_vec, n_of_inputs);
            }
            break;
            
            case 'R':
            case 'r': {
            ;
            }
            break;

            case 'W':
            case 'w': {
                printf("Wychodzenie z programu...\n");
                return 0;
            }
            break;

            default: break;
        }
    }

    



    return 0;
}
