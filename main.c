#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "expenses.txt"

typedef struct {
    char date[20];
    char name[50];
    float amount;
} Expense;

// Function prototypes
void addExpense();
void displayExpenses();
void removeExpense();
void getTotal();

int main() {
    int choice;

    while (1) {
        printf("\n==== Expense Tracker ====\n");
        printf("1. Add Expense(s)\n");
        printf("2. Display Expenses\n");
        printf("3. Remove Expense\n");
        printf("4. Get Total\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // clear newlines

        switch (choice) {
            case 1:
                addExpense();
                break;
            case 2:
                displayExpenses();
                break;
            case 3:
                removeExpense();
                break;
            case 4:
                getTotal();
                break;
            case 5:
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void addExpense() {
    FILE *file = fopen(FILE_NAME, "a");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    int n;
    Expense exp;

    // Get today's date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char today[20];
    sprintf(today, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    // Ask if user wants today's date
    char choice;
    printf("\nUse today's date (%s)? (y/n): ", today);
    scanf(" %c", &choice);
    getchar(); // clear newline

    if (choice == 'y' || choice == 'Y') {
        strcpy(exp.date, today);
    } else {
        printf("Enter date (YYYY-MM-DD): ");
        scanf("%19s", exp.date);
        getchar();
    }

    // Ask how many expenses
    printf("How many expenses do you want to add? ");
    scanf("%d", &n);
    getchar(); // clear newline

    for (int i = 0; i < n; i++) {
        printf("\nExpense %d:\n", i + 1);

        printf("Enter item name: ");
        fgets(exp.name, sizeof(exp.name), stdin);
        exp.name[strcspn(exp.name, "\n")] = '\0'; // remove newline

        printf("Enter amount: ");
        scanf("%f", &exp.amount);
        getchar();

        fprintf(file, "%s,%s,%.2f\n", exp.date, exp.name, exp.amount);
    }

    fclose(file);
    printf("\nAll expenses added successfully!\n");
}

void displayExpenses() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("No expenses found.\n");
        return;
    }

    int choice;
    char filter[20];
    Expense exp;
    char line[128];
    int index = 1;
    float total = 0;

    printf("\nDisplay Options:\n");
    printf("1. All expenses\n");
    printf("2. Expenses of a specific day (YYYY-MM-DD)\n");
    printf("3. Expenses of a specific month (YYYY-MM)\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 2) {
        printf("Enter date (YYYY-MM-DD): ");
        scanf("%19s", filter);
        getchar();
    } else if (choice == 3) {
        printf("Enter month (YYYY-MM): ");
        scanf("%19s", filter);
        getchar();
    }

    printf("\n%-5s %-12s %-20s %-10s\n", "ID", "Date", "Name", "Amount");
    printf("-------------------------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%19[^,],%49[^,],%f", exp.date, exp.name, &exp.amount);

        int show = 0;
        if (choice == 1) {
            show = 1;
        } else if (choice == 2 && strcmp(exp.date, filter) == 0) {
            show = 1;
        } else if (choice == 3 && strncmp(exp.date, filter, strlen(filter)) == 0) {
            show = 1;
        }

        if (show) {
            printf("%-5d %-12s %-20s %.2f\n", index++, exp.date, exp.name, exp.amount);
            total += exp.amount;
        }
    }

    if (index == 1) {
        printf("No matching expenses found.\n");
    } else {
        printf("-------------------------------------------------\n");
        printf("%-5s %-12s %-20s %.2f\n", "", "", "TOTAL", total);
    }

    fclose(file);
}


void removeExpense() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("No expenses to remove.\n");
        return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (!temp) {
        printf("Error creating temp file.\n");
        fclose(file);
        return;
    }

    displayExpenses();
    int removeID, currentID = 1;
    printf("\nEnter ID to remove: ");
    scanf("%d", &removeID);

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (currentID != removeID) {
            fputs(line, temp);
        }
        currentID++;
    }

    fclose(file);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.csv", FILE_NAME);

    printf("Expense removed successfully!\n");
}

void getTotal() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("No expenses found.\n");
        return;
    }

    Expense exp;
    char line[128];
    float total = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%19[^,],%49[^,],%f", exp.date, exp.name, &exp.amount);
        total += exp.amount;
    }

    fclose(file);

    printf("\nTotal Expenses: %.2f\n", total);
}
