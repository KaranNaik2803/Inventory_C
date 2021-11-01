#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "bill.h"

int countLeapYears(const int date[]) {
    int years = date[2];
    if (date[1] <= 2) years--;

    return (years/4 - years/100 + years/400);
}

long int get_days(const int date[], const int days_in_month[]) {
    long int n1 = date[2] * 365 + date[0];

    for (int i=0; i<date[1] - 1; i++) {
        n1 += days_in_month[i];
    }
    n1 += countLeapYears(date);

    return n1;
}

int get_difference_of_days(const int start_billing_date[], const int end_billing_date[], const int days_in_month[]) {
    long int n1 = get_days(start_billing_date, days_in_month);
    long int n2 = get_days(end_billing_date, days_in_month);

    return (n2 - n1 + 1);
}

void generate_time(const int no_of_bills, const bill_t *bills, const int *bill_index, const int date[], int time[]) {
    int increment = 660/no_of_bills;
    int prev_hour; int prev_mins; int min; int hour;
    if (*bill_index == 0 || date[0] != bills[*bill_index - 1].dd_) {
        // first customer or bill
        time[0] = 8;
        time[1] = 0;
    } else {
        prev_hour = bills[*bill_index - 1].hour_;
        prev_mins = (prev_hour*60) + bills[*bill_index - 1].min_;
        hour = prev_hour;
        min = (prev_mins + increment) % 60;
        // to increment hour
        if ((bills[*bill_index - 1].min_ + increment) >= 60) hour += 1;

        // 1pm - 4pm break
        if (hour == 13) hour = 16;
        
        // assign to bill
        time[0] = hour;
        time[1] = min;
    }
}

int find_unique_customer(const char names[][50], const int n, char temp_names[][50], int *k) {
    int same_name; int index;

    while(1) {
        same_name = 0;
        index = (rand() % n);
        // check if names[index] is not in temp_names;
        for(int x=0; x<*k; x++) {
            if (strcmp(names[index], temp_names[x]) == 0) {
                same_name = 1;
                break;
            }
        }
        if (same_name == 0) {
            strcpy(temp_names[*k], names[index]);
            *k += 1;
            break;
        }
    }

    return index;
}

int generate_item_list(const char customer_name[50], item_t *items, const int m, item_t *picked_items, const int time[], const int date[]) {
    FILE *file = fopen("./output/item_report.dat", "a");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }

    int no_of_items = (rand() % 5) + 1;
    char already_in_basket[no_of_items][50];
    int index;
    int b_index = 0;
    int i = 0;
    while(i < no_of_items) {
        // pick unique item
        index = pick_unique_item(items, m, i, already_in_basket);
        int quantity = (rand() % 5) + 1;
        // check if item is available in wanted quantity
        if (items[index].quantity_ < quantity) {
            // not enough quantity
            fprintf(file, "%2d %2d %4d\t\t%2d:%s%d %s\t%15s\t%15s\t%20d\t%20d\n",
                date[0], date[1], date[2],
                (time[0] <= 12) ? time[0] : time[0]%12,
                (time[1] < 10) ? "0" : "",
                time[1],
                (time[0] < 12) ? "am" : "pm",
                items[index].name_,
                customer_name,
                items[index].quantity_,
                quantity);
            no_of_items--;
        } else {
            // add item to basket
            picked_items[i] = items[index];
            picked_items[i].quantity_ = quantity;
            items[index].quantity_ -= quantity;
            strcpy(already_in_basket[b_index++], items[index].name_);
            i++;
        }
    }

    fclose(file);

    return no_of_items;
}

int pick_unique_item(const item_t *items, const int m, const int i, const char already_in_basket[][50]) {
    int index;
    int in_basket = 1;

    while(in_basket) {
        in_basket = 0;
        index = (rand() % m);
        int j;
        for (j=0; j<i; j++) {
            if (strcmp(items[index].name_, already_in_basket[j]) == 0) {
                // item is already in basket
                // => pick a different index (continue loop)
                in_basket = 1;
                break;
            }
        }
    }

    return index;
}

void generate_bill(const char names[][50], const int n, item_t *items, const int m, char temp_names[][50], int *k, const int no_of_bills, bill_t *bills, int *bill_index, const int date[]) {
    // pick a unique name
    int index;
    index = find_unique_customer(names, n, temp_names, k);
    
    // generate time
    int time[2];
    generate_time(no_of_bills, bills, bill_index, date, time);

    // pick 1-5 random items
    item_t picked_items[5]; int item_count;
    item_count = generate_item_list(names[index], items, m, picked_items, time, date);

    // no items => no bill
    if (item_count == 0) return;

    // calculate amount
    float amount = 0;
    for (int i=0; i<item_count; i++) {
        amount += (picked_items[i].price_ * picked_items[i].quantity_);
    }

    bills[*bill_index].id_ = *bill_index + 1;
    bills[*bill_index].dd_ = date[0];
    bills[*bill_index].mm_ = date[1];
    bills[*bill_index].yyyy_ = date[2];
    bills[*bill_index].hour_ = time[0];
    bills[*bill_index].min_ = time[1];
    strcpy(bills[*bill_index].customer_name_, names[index]);
    bills[*bill_index].item_count_ = item_count;
    for (int i=0; i<item_count; i++) {
        bills[*bill_index].items_[i] = picked_items[i];
    }
    bills[*bill_index].amount_ = amount;

    *bill_index += 1;
}

void get_next_date(int date[], const int days_in_month[]) {
    date[0] += 1;
    if (date[0] > days_in_month[date[1] - 1]) {
        if (date[1] == 12) {
            // go to jan 1st of next year
            date[1] = 1;
            date[2]++;
        } else {
            // go to next month
            date[1]++;
        }
        date[0] = 1;
    }
}

int skip_if_tuesday(int *curr_day, int date[], const int days_in_month[]) {
    if (*curr_day == 1) {
        get_next_date(date, days_in_month);
        *curr_day += 1;
        return 1;
    }

    return 0;
}

int bills_in_a_day(const int curr_day) {
    int r;

    if (curr_day == 5 || curr_day == 6) {
        r = (rand() % 21) + 40; // 40 - 60
    } else {
        r = (rand() % 21) + 20; // 20 - 40
    }

    return r;
}

void check_if_leap_year(int date[], int days_in_month[]) {
    if (date[2]%400 == 0 || (date[2]%100 != 0 && date[2]%4 == 0)) {
        days_in_month[1] = 29;
    } else {
        days_in_month[1] = 28;
    }
}

int populate_data(const char names[][50], const int n, item_t *items, const int m, bill_t *bills, const int start_billing_date[], const int end_billing_date[], const char day[]) {
    srand(time(NULL));
    const char day_of_week[7][5] = { "mon", "tue", "wed", "thur", "fri", "sat", "sun" };
    int days_in_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int date[] = { start_billing_date[0], start_billing_date[1], start_billing_date[2] };
    int total_days;
    int bill_index = 0;
    int curr_day;
    for (int i=0; i<7; i++) {
        if (strcmp(day, day_of_week[i]) == 0) {
            curr_day = i;
            break;
        }
    }

    FILE *file = fopen("./output/item_report.dat", "w");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }
    fprintf(file, "%10s\t\t", "DATE");
    fprintf(file, "%8s\t", "TIME");
    fprintf(file, "%15s\t", "ITEM NAME");
    fprintf(file, "%15s\t", "CUSTOMER NAME");
    fprintf(file, "%20s\t", "AVAILABLE QUANTITY");
    fprintf(file, "%20s\n\n", "REQUIRED QUANTITY");
    fclose(file);
    
    total_days = get_difference_of_days(start_billing_date, end_billing_date, days_in_month);
    for (int i=0; i<total_days; i++) {
        check_if_leap_year(date, days_in_month);
        if (skip_if_tuesday(&curr_day, date, days_in_month)) continue;
        int r = bills_in_a_day(curr_day);
        int no_of_bills = r;
        char temp_names[n][50]; int k = 0;
        while(r) {
            generate_bill(names, n, items, m, temp_names, &k, no_of_bills, bills, &bill_index, date);
            r--;
        }
        get_next_date(date, days_in_month);
        curr_day = (curr_day + 1) % 7;
    }

    return bill_index;
}

void display_bills(const bill_t *bills, const int n) {
    FILE *file = fopen("./output/bills.dat", "w");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }
    fprintf(file, "%6s\t\t", "BILL #");
    fprintf(file, "%10s\t\t", "DATE");
    fprintf(file, "%8s\t", "TIME");
    fprintf(file, "%18s\t", "CUSTOMER NAME");
    fprintf(file, "%15s\n\n", "AMOUNT");
    for (int i=0; i<n; i++) {
        fprintf(file, "%6d\t\t", bills[i].id_);
        fprintf(file, "%2d %2d %4d\t\t", bills[i].dd_, bills[i].mm_, bills[i].yyyy_);
        fprintf(file, "%2d:%s%d %s\t",
            (bills[i].hour_ <= 12) ? bills[i].hour_ : bills[i].hour_%12,
            (bills[i].min_ < 10) ? "0" : "",
            bills[i].min_,
            (bills[i].hour_ < 12) ? "am" : "pm");
        fprintf(file, "%18s\t", bills[i].customer_name_);
        fprintf(file, "%15.2f\n", bills[i].amount_);
    }

    fclose(file);
}

void display_detailed_bills(const bill_t *bills, const int n) {
    FILE *file = fopen("./output/detailed_bills.dat", "w");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }
    for (int i=0; i<n; i++) {
        fprintf(file, "BILL #: %3d\t\t\t", bills[i].id_);
        fprintf(file, "CUSTOMER NAME: %10s\t\t\t", bills[i].customer_name_);
        fprintf(file, "DATE: %2d %2d %4d\n\n", bills[i].dd_, bills[i].mm_, bills[i].yyyy_);
        fprintf(file, "\t%6s %18s %12s %14s %12s" ,"ITEM #", "ITEM NAME", "RATE", "QUANTITY", "AMOUNT\n");
        for (int j=0; j<bills[i].item_count_; j++) {
            float amount = bills[i].items_[j].price_ * bills[i].items_[j].quantity_;
            fprintf(file, "\t%4d %20s %12.2f %10d %15.2f\n", bills[i].items_[j].id_, bills[i].items_[j].name_, bills[i].items_[j].price_, bills[i].items_[j].quantity_, amount);
        }
        fprintf(file, "\n\t\tTOTAL AMOUNT: %10.2f\n\n", bills[i].amount_);
        fprintf(file, "----------------------------------------------------------------------------------------------\n\n");
    }

    fclose(file);
}

void total_by_date(const bill_t *bills, const int n) {
    FILE *file = fopen("./output/daily_amount.dat", "w");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }
    
    fprintf(file, "%8s\t", "DATE");
    fprintf(file, "%15s\n\n", "AMOUNT");

    int date[3] = { bills[0].dd_, bills[0].mm_, bills[0].yyyy_ };
    float total = bills[0].amount_;
    for (int i=1; i<n; i++) {
        if (bills[i].dd_ != date[0]) {
            // different day
            fprintf(file, "%2d %2d %4d\t%15.2f\n", date[0], date[1], date[2], total);
            date[0] = bills[i].dd_;
            date[1] = bills[i].mm_;
            date[2] = bills[i].yyyy_;
            total = bills[i].amount_;
        } else {
            // same day
            total += bills[i].amount_;
        }
    }
    fprintf(file, "%2d %2d %4d\t%15.2f\n", date[0], date[1], date[2], total);

    fclose(file);
}

void total_by_customer(const bill_t *bills, const int n, const int MAX_CUSTOMERS) {
    FILE *file = fopen("./output/cust_amount.dat", "w");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }

    fprintf(file, "%15s\t", "CUSTOMER NAME");
    fprintf(file, "%15s\n\n", "AMOUNT");

    float customer_amount[MAX_CUSTOMERS];
    char customer_names[MAX_CUSTOMERS][50];
    int index_amount = 0;
    int index_name = 0;

    for (int i=0; i<n; i++) {
        int j;
        for (j=0; j<index_name; j++) {
            if (strcmp(bills[i].customer_name_, customer_names[j]) == 0) {
                break;
            }
        }
        if (j == index_name) {
            // new customer
            strcpy(customer_names[index_name++], bills[i].customer_name_);
            customer_amount[index_amount++] = bills[i].amount_;
        } else {
            // existing customer
            customer_amount[j] += bills[i].amount_;
        }
    }

    for (int i=0; i<index_name; i++) {
        fprintf(file, "%15s\t", customer_names[i]);
        fprintf(file, "%15.2f\n", customer_amount[i]);
    }

    fclose(file);
}

void sales_by_day(const bill_t *bills, const int n, const char day[]) {
    FILE *file = fopen("./output/sales_by_day.dat", "w");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }

    const char day_of_week[7][5] = { "mon", "tue", "wed", "thur", "fri", "sat", "sun" };
    float amount[7] = { 0 };
    int curr_day;
    for (int i=0; i<7; i++) {
        if (strcmp(day, day_of_week[i]) == 0) {
            curr_day = i;
            break;
        }
    }
    // skip if starting date is tuesday
    if (curr_day == 1) curr_day += 1;
    amount[curr_day] = bills[0].amount_;
    for (int i=1; i<n; i++) {
        // new date
        if (bills[i].dd_ != bills[i - 1].dd_) {
            curr_day = (curr_day + 1) % 7;
            // skip tuesdays
            if (curr_day == 1) curr_day += 1;
        }
        amount[curr_day] += bills[i].amount_;
    }

    fprintf(file, "%5s\t", "DAY");
    fprintf(file, "%15s\n\n", "AMOUNT");

    for (int i=0; i<7; i++) {
        fprintf(file, "%5s\t", day_of_week[i]);
        fprintf(file, "%15.2f\n", amount[i]);
    }

    fclose(file);
}

void sales_by_hour(const bill_t *bills, const int n) {
    FILE *file = fopen("./output/sales_by_hour.dat", "w");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }

    char time_slots[11][15] = { "8am - 9am", "9am - 10am", "10am - 11am", "11am - 12am", "12pm - 1pm",
        "4pm - 5pm", "5pm - 6pm", "6pm - 7pm", "7pm - 8pm", "8pm - 9pm", "9pm - 10pm" };

    float hourly_amount[11] = { 0 };
    int mins;
    for (int i=0; i<n; i++) {
        mins = (bills[i].hour_ * 60) + bills[i].min_;
        if (mins >= 8*60 && mins < 9*60) {
            hourly_amount[0] += bills[i].amount_;
        } else if (mins >= 9*60 && mins < 10*60) {
            hourly_amount[1] += bills[i].amount_;
        } else if (mins >= 10*60 && mins < 11*60) {
            hourly_amount[2] += bills[i].amount_;
        } else if (mins >= 11*60 && mins < 12*60) {
            hourly_amount[3] += bills[i].amount_;
        } else if (mins >= 12*60 && mins < 13*60) {
            hourly_amount[4] += bills[i].amount_;
        } else if (mins >= 16*60 && mins < 17*60) {
            hourly_amount[5] += bills[i].amount_;
        } else if (mins >= 17*60 && mins < 18*60) {
            hourly_amount[6] += bills[i].amount_;
        } else if (mins >= 18*60 && mins < 19*60) {
            hourly_amount[7] += bills[i].amount_;
        } else if (mins >= 19*60 && mins < 20*60) {
            hourly_amount[8] += bills[i].amount_;
        } else if (mins >= 20*60 && mins < 21*60) {
            hourly_amount[9] += bills[i].amount_;
        } else if (mins >= 21*60 && mins < 22*60) {
            hourly_amount[10] += bills[i].amount_;
        }
    }

    fprintf(file, "%15s\t", "TIME");
    fprintf(file, "%15s\n\n", "AMOUNT");
    for (int i=0; i<11; i++) {
        fprintf(file, "%15s\t", time_slots[i]);
        fprintf(file, "%15.2f\n", hourly_amount[i]);
    }

    fclose(file);
}

void item_sales_datewise(const bill_t *bills, const int n, const int m) {
    FILE *file = fopen("./output/item_sales_datewise.dat", "w");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }

    int date[3] = { bills[0].dd_, bills[0].mm_, bills[0].yyyy_ };
    // items of a day
    item_t temp_items[m];
    int index = 0;

    for (int i=0; i<n; i++) {
        if (bills[i].dd_ != date[0]) {
            // different day
            fprintf(file, "DATE : %2d %2d %4d\n\n", date[0], date[1], date[2]);
            fprintf(file, "%15s %15s %15s\n\n", "ITEM NAME", "QUANTITY", "AMOUNT");
            for (int x=0; x<index; x++) {
                fprintf(file, "%15s %15d %15.2f\n", temp_items[x].name_, temp_items[x].quantity_, temp_items[x].price_);
            }
            fprintf(file, "----------------------------------------------------------------------------------------------\n\n");
            memset(temp_items, 0, sizeof(temp_items));
            date[0] = bills[i].dd_;
            date[1] = bills[i].mm_;
            date[2] = bills[i].yyyy_;
            index = 0;
            for (int j=0; j<bills[i].item_count_; j++) {
                // check if new item in the day
                int k;
                for (k=0; k<index; k++) {
                    if (strcmp(bills[i].items_[j].name_, temp_items[k].name_) == 0) {
                        temp_items[k].price_ += (bills[i].items_[j].price_ * bills[i].items_[j].quantity_);
                        temp_items[k].quantity_ += bills[i].items_[j].quantity_;
                        break;
                    }
                }
                // else add new item with price
                if (k == index) {
                    temp_items[index] = bills[i].items_[j];
                    temp_items[index].quantity_ = bills[i].items_[j].quantity_;
                    temp_items[index++].price_ = (bills[i].items_[j].price_ * bills[i].items_[j].quantity_);
                }
            }
        } else {
            // same day
            // loop through the bill's items

            for (int j=0; j<bills[i].item_count_; j++) {
                // check if new item in the day
                int k;
                for (k=0; k<index; k++) {
                    if (strcmp(bills[i].items_[j].name_, temp_items[k].name_) == 0) {
                        temp_items[k].price_ += (bills[i].items_[j].price_ * bills[i].items_[j].quantity_);
                        temp_items[k].quantity_ += bills[i].items_[j].quantity_;
                        break;
                    }
                }
                // else add new item with price
                if (k == index) {
                    temp_items[index] = bills[i].items_[j];
                    temp_items[index].quantity_ = bills[i].items_[j].quantity_;
                    temp_items[index++].price_ = (bills[i].items_[j].price_ * bills[i].items_[j].quantity_);
                }
            }
        }
    }
    fprintf(file, "DATE : %2d %2d %4d\n\n", date[0], date[1], date[2]);
    fprintf(file, "%15s %15s %15s\n\n", "ITEM NAME", "QUANTITY", "AMOUNT");
    for (int x=0; x<index; x++) {
        fprintf(file, "%15s %15d %15.2f\n", temp_items[x].name_, temp_items[x].quantity_, temp_items[x].price_);
    }
    fprintf(file, "----------------------------------------------------------------------------------------------\n\n");

    fclose(file);
}

int date_to_be_tracked(const int date[], const int start_billing_date[], const int end_billing_date[], const int days_in_month[]) {

    long int start_days = get_days(start_billing_date, days_in_month);
    long int end_days = get_days(end_billing_date, days_in_month);
    long int curr_days = get_days(date, days_in_month);

    return (curr_days >= start_days && curr_days <= end_days) ? 1 : 0;
}

void item_sales_periodwise(const bill_t *bills, const int n, const int m, const int start_billing_date[], const int end_billing_date[]) {
    FILE *file = fopen("./output/item_sales_periodwise.dat", "w");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }
    item_t temp_items[m];
    int index = 0;
    int date[3];
    int days_in_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int tracking = 0;

    for (int i=0; i<n; i++) {
        date[0] = bills[i].dd_; date[1] = bills[i].mm_; date[2] = bills[i].yyyy_;
        if (date_to_be_tracked(date, start_billing_date, end_billing_date, days_in_month)) {
            for (int j=0; j<bills[i].item_count_; j++) {
                // check if new item in the day
                int k;
                for (k=0; k<index; k++) {
                    if (strcmp(bills[i].items_[j].name_, temp_items[k].name_) == 0) {
                        temp_items[k].quantity_ += bills[i].items_[j].quantity_;
                        temp_items[k].price_ += (bills[i].items_[j].price_ * bills[i].items_[j].quantity_);
                        break;
                    }
                }
                // else add new item with price
                if (k == index) {
                    temp_items[index] = bills[i].items_[j];
                    temp_items[index].quantity_ = bills[i].items_[j].quantity_;
                    temp_items[index++].price_ = (bills[i].items_[j].price_ * bills[i].items_[j].quantity_);
                }
            }
        }
    }

    fprintf(file, "START DATE : %2d %2d %4d %10s END DATE : %2d %2d %4d\n",
        start_billing_date[0], start_billing_date[1], start_billing_date[2], " ",
        end_billing_date[0], end_billing_date[1], end_billing_date[2]);
    fprintf(file, "------------------------------------------------------------------\n\n");
    if (index == 0) {
        fprintf(file, "NO ITEM SALES IN THIS RANGE.");
    } else {
        fprintf(file, "%15s %15s %15s\n\n", "ITEM NAME", "QUANTITY", "AMOUNT");
        for (int i=0; i<index; i++) {
            fprintf(file, "%15s %15d %15.2f\n", temp_items[i].name_, temp_items[i].quantity_, temp_items[i].price_);
        }
    }

    fclose(file);
}

void item_inventory(const item_t *items, const int item_count) {
    FILE *file = fopen("./output/item_inventory.dat", "w");
    if (!file) {
        printf("Error! opening file");
        exit(1);
    }

    for (int i=0; i<item_count; i++) {
        fprintf(file, "%15s => %3d\n", items[i].name_, items[i].quantity_);
    }

    fclose(file);
}