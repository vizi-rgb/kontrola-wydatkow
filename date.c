#include <stdio.h>
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

    return -2; 
}
