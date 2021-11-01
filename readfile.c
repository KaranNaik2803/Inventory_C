#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readfile.h"

int read_names(char names[][50]) {
    char name[50];
    int i = 0;
    FILE *file = fopen("./input/names.dat", "r");

    if (!file) {
        printf("Error! opening file");
        exit(1);
    }

    while(fscanf(file, "%s", name) != EOF) {
        strcpy(names[i++], name);
    }

    if (i == 0) {
        printf("Empty file.\n");
        exit(1);
    }

    fclose(file);

    return i;
}

void read_dates(int start_billing_date[], int end_billing_date[], char day[]) {
    int date;
    FILE *file = fopen("./input/dates.dat", "r");

    if (!file) {
        printf("Error! opening file");
        exit(1);
    }

    const char day_of_week[7][5] = { "mon", "tue", "wed", "thur", "fri", "sat", "sun" };

    for (int i=0; i<3; i++) {
        fscanf(file, "%d", &date);
        start_billing_date[i] = date;
    }
    fscanf(file, "%s", day);
    
    for (int i=0; i<3; i++) {
        fscanf(file, "%d", &date);
        end_billing_date[i] = date;
    }

    int ind;
    for (ind=0; ind<7; ind++) {
        if (strcmp(day, day_of_week[ind]) == 0) break;
    }

    if (ind == 7) {
        printf("Invalid day format, please check input.\n");
        exit(1);
    }

    fclose(file);
}

void check_dates(int start_billing_date[], int end_billing_date[]) {
    int wrong_date = 0;
    int s_dd; int s_mm; int s_yy; int e_dd; int e_mm; int e_yy;
    s_dd = start_billing_date[0]; s_mm = start_billing_date[1]; s_yy = start_billing_date[2];
    e_dd = end_billing_date[0]; e_mm = end_billing_date[1]; e_yy = end_billing_date[2];
    int days_in_month_start[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int days_in_month_end[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    check_if_leap_year(start_billing_date, days_in_month_start);
    check_if_leap_year(end_billing_date, days_in_month_end);
    
    if (s_yy > e_yy || (s_yy == e_yy && s_mm > e_mm) || (s_yy == e_yy && s_mm == e_mm && s_dd > e_dd)) wrong_date = 1;
    if (s_mm > 12 || e_mm > 12) wrong_date = 1;
    if (s_dd > days_in_month_start[s_mm - 1] || e_dd > days_in_month_end[e_mm - 1]) wrong_date = 1;

    if (wrong_date) {
        printf("Invalid start and end dates.\n");
        exit(1);
    }
}

int read_items(item_t *items) {
    char item[50];
    float price;
    int quantity;
    int i = 0;
    FILE *file = fopen("./input/items.dat", "r");

    if (!file) {
        printf("Error! opening file");
        exit(1);
    }

    while(fscanf(file, "%s", item) != EOF) {
        fscanf(file, "%f", &price);
        fscanf(file, "%d", &quantity);
        strcpy(items[i].name_, item);
        items[i].price_ = price;
        items[i].quantity_ = quantity;
        items[i].id_ = i + 1;
        i++;
    }

    if (i == 0) {
        printf("Empty file.\n");
        exit(1);
    }

    fclose(file);

    return i;
}
