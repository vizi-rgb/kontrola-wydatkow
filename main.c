#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef FILE_NAME
    #define FILE_NAME "data"
#endif

int check_date(FILE *file, int month, int year) {
    char c;
    int m, y;
    // 0 the date is already in the file
    // 1 the date is not in the file
    
    fopen(file, "r");

    while ((c = fgetc(file)) != EOF) {
        if (c == '-' && (c == fgetc(file)) == '-') {
            fscanf(file, "%d %d", &m, &y);
            if (month == m && year == y) {
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);
    return 1;

}

void add_sum(FILE *file, int month, int year, double sum[], int n) {
    if (check_date(file, month, year) == 1) {
        fopen(file, "a");

        while (n--) {
            fprintf(file, "%lf\n", sum[n]);
        }
        fclose(file);
    } else if (check_date(file, month, year) == 0) {
        fopen(file, "a");
        fprintf(file, "-- %d %d\n", month, year);
        while (n--) {
            fprintf(file, "%lf\n", sum[n]);
        }
        fclose(file);

    }
}


void show_report(FILE *file, int month, int year) {
;
}



int main(int argc, char **argv) {
    FILE *file;
    int choice;

    do {
        fprintf(stdout,  "\t[D]odaj kwote\n"
                        "\t[R]aport\n"
                        "\t[W]yjdz\n");

    } while (fscanf(stdin, "%d", &choice) > 0);


    return 0;
}
