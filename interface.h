struct item {
    char name_[50];
    int quantity_;
    float price_;
};
typedef struct item item_t;

struct bill {
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

//---------------------------------------------------------------------------------------
// read customer names
int read_names(char names[][50]);
/*
This function reads customer names from the file `names.dat`

Input :
    names => an array to store the names that are read from the file
Output :
    customer names
Return :
    number of customers
*/

//---------------------------------------------------------------------------------------
// read date range
void read_dates(int start_billing_date[], int end_billing_date[], char day[]);
/*
This function reads starting date and the end date from the file `dates.dat`

Input :
    start_billing_date => an array to store start date that is read from the file
    end_billing_date => an array to store end date that is read from the file
    day => string to store the starting day of the week
Output :
    start date, end date and start day
Return :
    None
*/

//---------------------------------------------------------------------------------------
// to check validity of the input dates
void check_dates(int start_billing_date[], int end_billing_date[]);
/*
This function check if the start and end dates are valid

Input :
    start_billing_date => an array to store start date that is read from the file
    end_billing_date => an array to store end date that is read from the file
Output :
    exit the program if invalid dates
    continue if valid.
Return :
    None
*/

//---------------------------------------------------------------------------------------
// read items from file
int read_items(item_t *items);
/*
This function reads items from the file `items.dat`

Input :
    items => an array of structures to store the item name and unit price
Output :
    array of structure elements of all items
Return :
    number of items
*/

//---------------------------------------------------------------------------------------
// count the number of leap years upto the current year
int countLeapYears(const int date[]);
/*
This function count the number of leap years uptil the current year

Input :
    date =>
        - date[0] : date; date[1] : month; date[2] : year
Output :
    count of leap years
Return :
    count of leap years
*/

//---------------------------------------------------------------------------------------
// get the number of days for a date
long int get_days(const int date[], const int days_in_month[]);
/*
This function finds the total number of days from start of time to the given date.
internally calls countLeapYears()

Input :
    start_billing_date =>
        - start_billing_date[0] : date; start_billing_date[1] : month; start_billing_date[2] : year;
    end_billing_date =>
        - end_billing_date[0] : date; end_billing_date[1] : month; end_billing_date[2] : year;
Output :
    number of days from start of time upto date
Return :
    number of days upto date
*/

//---------------------------------------------------------------------------------------
// get the difference of days between 2 dates
int get_difference_of_days(const int start_billing_date[], const int end_billing_date[], const int days_in_month[]);
/*
This function finds the total number of days between start_billing_date and end_billing_date.

Input :
    start_billing_date =>
        - start_billing_date[0] : date; start_billing_date[1] : month; start_billing_date[2] : year;
    end_billing_date =>
        - end_billing_date[0] : date; end_billing_date[1] : month; end_billing_date[2] : year;
    days_in_month => array consisting of number of days in each month
Output :
    number of days between start_billing_date and end_billing_date
Return :
    number of days between start_billing_date and end_billing_date
*/

//---------------------------------------------------------------------------------------
void generate_time(const int no_of_bills, const bill_t *bills, const int *bill_index, const int date[], int time[]);
/*
This function does the following:
    generate minutes(random, 1 - 20 mins)
    assign the starting time to first customer
    checks for break time from 1pm - 4pm
    store the generated time in `time`

Input :
    no_of_bills => number of bills for the day
    bills => array of structure elements of bills
    bill_index => index to add the new generated bill
    date => current date
    time => time to be assigned to the bill
Output :
    time is generated for the current bill
Return :
    None
*/

//---------------------------------------------------------------------------------------
// to find a unique customer for the day
int find_unique_customer(const char names[][50], const int n, char temp_names[][50], int *k);
/*
    This function does the following:
        generates unique customer for each day

Input :
    names => array of customer names
    n => size of names
    temp_names => temporary array of customer names in a day
    k => size of temp_names
Output :
    Index of the unique customer
Return :
    Index of the unique customer
*/

//---------------------------------------------------------------------------------------
// generate items using random index of items
int generate_item_list(item_t *items, const int m, item_t *picked_items);
/*
This function generates random item from the items.dat file based on the random index generated

Input :
    items => array of structure elements of all items
    m => size of items
    picked_items => array of structure elements of items picked
Output :
    randomly picks 1-5 items from the item list
Return :
    count of items
*/

//---------------------------------------------------------------------------------------
int pick_unique_item(const item_t *items, const int m, const int i, const char already_in_basket[][50]);
/*
This function compares the randomly picked item with items already in the bill(basket)
and keeps looping till a unique index is found

Input :
    items => array of structure elements of all items
    m => size of items
    i => count of items already in basket
    already_in_basket => names of items in the basket
Output :
    a unique item is picked
Return :
    index of unique item
*/

//---------------------------------------------------------------------------------------
// generate information using random
void generate_bill(const char names[][50], const int n, item_t *items, const int m,
    char temp_names[][50], int *k, const int no_of_bills, bill_t *bills, int *bill_index, const int date[]);
/*
This function does the following:
    pick a name from array `names` (random, unique customer of the day)
    store customers of the day in temp_names
    generate amount (random, Rs.100 - Rs.5000)
    add the bill to bills array

Input :
    names => array of customer names
    n => size of names
    items => array of structure elements of all items
    m => size of items
    temp_names => temporary array of customer names in a day
    k => size of temp_names
    bills => array of structure elements of bills
    bill_index => index to add the new generated bill
    date => current date
Output :
    bill is generated and stored in bills array
Return :
    None
*/

//---------------------------------------------------------------------------------------
// to obtain the next valid date
void get_next_date(int date[], const int days_in_month[]);
/*
This function generates the next closest valid date.

Input :
    date =>
        - date[0] : date; date[1] : month; date[2] : year
    days_in_month => array consisting of number of days in each month
Output :
    valid date
Return :
    None
*/

//---------------------------------------------------------------------------------------
// to skip the current date if it's a tuesday
int skip_if_tuesday(int *curr_day, int date[], const int days_in_month[]);
/*
This function checks whether the current day is a Tuesday
If it is, call `get_valid_date` to get the next valid day and move to Wednesday

Input :
    curr_day => 0-based indexing of the current day of the week ex: 0 = "mon", 1 = "tues", ...
    date =>
        - date[0] : date; date[1] : month; date[2] : year
    days_in_month => array consisting of number of days in each month
Output :
    skips tuesdays
Return :
    True or False
*/

//---------------------------------------------------------------------------------------
int bills_in_a_day(const int curr_day);
/*
This function generates the number of bills that can be made for a day
 based on the Saturday and Sunday higher sales constraint.

Input :
    curr_day => 0-based indexing of the current day of the week ex: 0 = "mon", 1 = "tues", ...
Output :
    number of bills to be generated for a particular day
Return :
    int => number of bills to be generated for a particular day
*/

//---------------------------------------------------------------------------------------
// to check if the current year is a leap year
void check_if_leap_year(int date[], int days_in_month[]);
/*
This function checks if the current year is a leap year

Input :
    date =>
        - date[0] : date; date[1] : month; date[2] : year
    days_in_month => array consisting of number of days in each month
Output :
    changes the number of days in february to 29 if leap year
Return :
    None
*/

//---------------------------------------------------------------------------------------

int populate_data(const char names[][50], const int n, item_t *items, const int m,
    bill_t *bills, const int start_billing_date[], const int end_billing_date[], const char day[]);
/*
This function does the following:
        loop from start_billing_date to end_billing_date
        keep the same date for 20 - 40 iterations (random)
        calls generate_bill();
        next iteration
        
Input :
    names => array of customer names
    n => size of names
    items => array of structure elements of all items
    m => size of items
    bills => array of structure elements of bills
    start_billing_date => start range of bills
    end_billing_date => end range of bills
    day => starting day of the week
Output :
    array of structures
Return :
    number of bills
*/

//---------------------------------------------------------------------------------------

// generate report of all the bills
void display_bills(const bill_t *bills, const int n);
/*
This function loops through the bills and outputs it to `bills.dat`

Input :
    bills => array of structure elements of bills
    n => size of bills
Output :
    `bills.dat`
Return :
    None
*/

//---------------------------------------------------------------------------------------
void display_detailed_bills(const bill_t *bills, const int n);
/*
This function loops through the bills and generates a detailed bill and
writes it to `detailed_bills.dat`

Input :
    bills => array of structure elements of bills
    n => size of bills
Output :
    `detailed_bills.dat`
Return :
    None
*/

//---------------------------------------------------------------------------------------
// generate report of total amount spent in a dayspent
void total_by_date(const bill_t *bills, const int n);
/*
This function loops through the array of structure elements and calculates the total amount spent in a day
and writes it to `daily_amount.dat`

Input :
    bills => array of structure elements of bills
    n => size of bills
Output :
    `daily_amount.dat`
Return :
    None
*/

//---------------------------------------------------------------------------------------
// generate report of total amount spent by a customer
void total_by_customer(const bill_t *bills, const int n, const int MAX_CUSTOMERS);
/*
This function loops through the array of structure elements and calculates
 the total amount spent by a customer and writes it to `cust_amount.dat`

Input :
    bills => array of structure elements of bills
    n => size of bills
Output :
    `cust_amount.dat`
Return :
    None
*/

//---------------------------------------------------------------------------------------
// compare sales by day
void sales_by_day(const bill_t *bills, const int n, const char day[]);
/*
This function loops through the array of structure elements and compares sales by day of the week
and writes it to `sales_by_day.dat`

Input :
    bills => array of structure elements of bills
    n => size of bills
    day => string to store the starting day of the week
Output :
    `sales_by_day.dat`
Return :
    None
*/

//---------------------------------------------------------------------------------------
// compare sales by hour
void sales_by_hour(const bill_t *bills, const int n);
/*
This function loops through the array of structure elements and compares sales by
 working hours of the day and writes it to `sales_by_amount.dat`

Input :
    bills => array of structure elements of bills
    n => size of bills
Output :
    `sales_by_amount.dat`
Return :
    None
*/

//---------------------------------------------------------------------------------------

void item_sales_datewise(const bill_t *bills, const int n, const int m);
/*
This function loops through the bills and calculates the sales of items in each day
and writes it to `item_sales_datewise.dat`

Input :
    bills => array of structure elements of bills
    n => size of bills
    m => count of total items
Output :
    `item_sales_datewise.dat`
Return :
    None
*/

//---------------------------------------------------------------------------------------

void item_sales_periodwise(const bill_t *bills, const int n, const int m,
    const int start_billing_date[], const int end_billing_date[]);
/*
This function loops through the bill and calculates the items sales during a period
 and writes it to `item_sales_periodwise.dat`

Input :
    bills => array of structure elements of bills
    n => size of bills
    m => count of total items
    start_billing_date => start range of bills
    end_billing_date => end range of bills
Output :
    `item_sales_periodwise.dat`
Return :
    None
*/

//---------------------------------------------------------------------------------------
void item_inventory(const item_t *items, const int item_count);
/*
This function loops through the items and prints the quantity left.
writes it to `item_inventory.dat`

Input :
    items => array of structure elements of all items
    item_count => size of items
Output :
    `item_inventory.dat`
Return :
    None
*/

//---------------------------------------------------------------------------------------
