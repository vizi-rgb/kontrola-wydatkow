#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BUF_SIZE 100

#ifndef FILE_NAME
    #define FILE_NAME "data"
#endif


int date_check(struct tm *time_info) {
    FILE *we = fopen(FILE_NAME, "r");

    if (we == NULL)
        return 0;

    char buf[BUF_SIZE];
    int month, year;


    while (fgets(buf, BUF_SIZE, we) != NULL) {
        if (buf[0] == '-') {
            if (sscanf(buf + 3, "%d.%d", &month, &year) != 2)
                return -2;

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

long int date_find(struct tm *time_info) {
    FILE *in = fopen(FILE_NAME, "r");
    char buf[BUF_SIZE];
    long int cur_pos = -1;
    int month, year;

    if (in == NULL) 
        return -1;

    while (fgets(buf, BUF_SIZE, in) != NULL) {
        if (buf[0] == '-') {
            if (sscanf(buf + 3, "%d.%d", &month, &year) != 2)
                return -2; // formatting problem occured in the txt file 

            if (time_info->tm_mon == month && time_info->tm_year + 1900 == year) {
                cur_pos = ftell(in);
                fclose(in);
                return cur_pos;
            }
        }

    }

    return 0; 
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
        fprintf(out, "%.2lf\n", money_vec[i]);

    fclose(out);

}


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
