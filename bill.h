#ifndef BILL_H
#define BILL_H

struct item {
    int id_;
    char name_[50];
    int quantity_;
    float price_;
};
typedef struct item item_t;

struct bill {
    int id_;
    int dd_;
    int mm_;
    int yyyy_;
    int hour_;
    int min_;
    char customer_name_[50];
    item_t items_[5];
    int item_count_;
    float amount_;
};
typedef struct bill bill_t;


// logic functions
int countLeapYears(const int date[]);

long int get_days(const int date[], const int days_in_month[]);

int get_difference_of_days(const int start_billing_date[], const int end_billing_date[], const int days_in_month[]);

void generate_time(const int no_of_bills, const bill_t *bills, const int *bill_index, const int date[], int time[]);

int find_unique_customer(const char names[][50], const int n, char temp_names[][50], int *k);

int generate_item_list(const char customer_name[50], item_t *items, const int m, item_t *picked_items, const int time[], const int date[]);

int pick_unique_item(const item_t *items, const int m, const int i, const char already_in_basket[][50]);

void generate_bill(const char names[][50], const int n, item_t *items, const int m,
    char temp_names[][50], int *k, const int no_of_bills, bill_t *bills, int *bill_index, const int date[]);

void get_next_date(int date[], const int days_in_month[]);

int skip_if_tuesday(int *curr_day, int date[], const int days_in_month[]);

int bills_in_a_day(const int curr_day);

void check_if_leap_year(int date[], int days_in_month[]);

int populate_data(const char names[][50], const int n, item_t *items, const int m,
    bill_t *bills, const int start_billing_date[], const int end_billing_date[], const char day[]);


// output functions
void display_bills(const bill_t *bills, const int n);

void display_detailed_bills(const bill_t *bills, const int n);

void total_by_date(const bill_t *bills, const int n);

void total_by_customer(const bill_t *bills, const int n, const int MAX_CUSTOMERS);

void sales_by_day(const bill_t *bills, const int n, const char day[]);

void sales_by_hour(const bill_t *bills, const int n);

void item_sales_datewise(const bill_t *bills, const int n, const int m);

void item_sales_periodwise(const bill_t *bills, const int n, const int m,
    const int start_billing_date[], const int end_billing_date[]);

void item_inventory(const item_t *items, const int item_count);

#endif