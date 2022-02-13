#include <stdio.h>
#include <stdlib.h>

#include "date.h"
#include "config.h"

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

long int date_find(int m, int y) {
    FILE *in = fopen(FILE_NAME, "r");
    char buf[BUF_SIZE];
    long int cur_pos = -1;
    int month, year;

    if (in == NULL) 
        return -1;

    while (fgets(buf, BUF_SIZE, in) != NULL) {
        if (buf[0] == '-') {
            if (sscanf(buf + 3, "%d.%d", &month, &year) != 2) {
                fclose(in);
                return -2; // formatting problem occured in the txt file 
            }

            if (m == month && y == year) {
                cur_pos = ftell(in);
                fclose(in);
                return cur_pos;
            }
        }

    }
    fclose(in);
    return -2; 
}

int *date_list(int *n) {
    FILE *in = fopen(FILE_NAME, "r");
    char buf[BUF_SIZE];
    int size = 20;
    int *dates_in_file = malloc(sizeof *dates_in_file * size); 
    // dates_in_file = { 1, 2002  month, year
    //                   3, 2003}

    if (in == NULL) 
        return NULL;

    while (fgets(buf, BUF_SIZE, in) != NULL) {
        if (buf[0] == '-') {
            sscanf(buf + 3, "%d.%d", dates_in_file + *n, dates_in_file + *n + 1);
            (*n) += 2;

            if (*n == size - 2) {
                size *= 2;
                dates_in_file = realloc(dates_in_file, sizeof *dates_in_file * size);
            } 
        }
    }

    fclose(in);
    dates_in_file = realloc(dates_in_file, sizeof *dates_in_file * *n);
    return dates_in_file;

}
