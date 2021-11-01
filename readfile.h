#ifndef READFILE_H
#define READFILE_H
#include "bill.h"

int read_names(char names[][50]);
void read_dates(int start_billing_date[], int end_billing_date[], char day[]);
void check_dates(int start_billing_date[], int end_billing_date[]);
int read_items(item_t *items);

#endif