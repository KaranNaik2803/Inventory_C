#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readfile.h"
#include "bill.h"
#define MAX_BILLS 1000
#define MAX_CUSTOMERS 90
#define MAX_ITEMS 60

int main() {
    item_t items[MAX_ITEMS];
    bill_t bills[MAX_BILLS];
    char names[MAX_CUSTOMERS][50];
    int start_billing_date[3]; int end_billing_date[3];
    int no_of_bills;
    char day[10];
    int customer_count; int item_count;

    customer_count = read_names(names);
    read_dates(start_billing_date, end_billing_date, day);
    check_dates(start_billing_date, end_billing_date);
    printf("START DATE : %d-%d-%d\n", start_billing_date[0], start_billing_date[1], start_billing_date[2]);
    printf("END DATE : %d-%d-%d\n", end_billing_date[0], end_billing_date[1], end_billing_date[2]);

    item_count = read_items(items);
    no_of_bills = populate_data(names, customer_count, items, item_count, bills, start_billing_date, end_billing_date, day);
    printf("Number of bills : %d\n", no_of_bills);
    display_bills(bills, no_of_bills);
    display_detailed_bills(bills, no_of_bills);
    total_by_date(bills, no_of_bills);
    total_by_customer(bills, no_of_bills, customer_count);
    sales_by_day(bills, no_of_bills, day);
    sales_by_hour(bills, no_of_bills);
    item_sales_datewise(bills, no_of_bills, item_count);

    int period_start[3]; int period_end[3];
    printf("Enter the start date to view item sales (d m yyyy) : ");
    scanf("%d %d %d", &period_start[0], &period_start[1], &period_start[2]);
    printf("Enter the end date to view item sales (d m yyyy) : ");
    scanf("%d %d %d", &period_end[0], &period_end[1], &period_end[2]);
    check_dates(period_start, period_end);
    item_sales_periodwise(bills, no_of_bills, item_count, period_start, period_end);
    item_inventory(items, item_count);
    printf("Executed successfully\n");
}