#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDERS 100

// ANSI color codes
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

typedef struct {
    char restaurant[50];
    char item[50];
    char location[100];
} Order;

Order orders[MAX_ORDERS];
int orderCount = 0;

void createCSV() {
    char filePath[] = "C:\\Users\\bazinga\\Desktop\\project\\orders.csv";
    FILE *file = fopen(filePath, "a");

    if (file == NULL) {
        printf(RED "Error opening file for writing. Please check the file path.\n" RESET);
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize == 0) {
        fprintf(file, "Restaurant,Item,Location\n");
    }

    for (int i = 0; i < orderCount; i++) {
        fprintf(file, "%s,%s,%s\n", orders[i].restaurant, orders[i].item, orders[i].location);
    }

    fclose(file);
    printf(GREEN "CSV file updated successfully at: %s\n" RESET, filePath);
}

void readCSV() {
    char filePath[] = "C:\\Users\\bazinga\\Desktop\\project\\orders.csv";
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        printf(YELLOW "No previous orders found (file not created yet).\n" RESET);
        return;
    }

    char buffer[200];
    fgets(buffer, sizeof(buffer), file);

    while (fgets(buffer, sizeof(buffer), file)) {
        Order newOrder;
        sscanf(buffer, "%49[^,],%49[^,],%99[^\n]", newOrder.restaurant, newOrder.item, newOrder.location);
        orders[orderCount++] = newOrder;
    }

    fclose(file);
    printf(GREEN "Previous orders loaded successfully.\n" RESET);
}

void displayOrders() {
    if (orderCount == 0) {
        printf(RED "\nNo previous orders found.\n" RESET);
    } else {
        printf(BLUE "\nPrevious Orders:\n" RESET);
        printf("Restaurant, Item, Location\n");
        for (int i = 0; i < orderCount; i++) {
            printf(GREEN "%s" RESET ", " YELLOW "%s" RESET ", " BLUE "%s" RESET "\n",
                   orders[i].restaurant, orders[i].item, orders[i].location);
        }
    }
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int getValidatedIntInput(int min, int max, const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) != 1) {
            while (getchar() != '\n');
            printf(RED "Invalid input. Please enter a number.\n" RESET);
        } else if (value < min || value > max) {
            printf(RED "Choice out of range (%d-%d). Try again.\n" RESET, min, max);
        } else {
            return value;
        }
    }
}

void getValidatedStringInput(char *buffer, int size, const char *prompt) {
    printf("%s", prompt);
    scanf("%99s", buffer);
}

int main() {
    char location[100];
    int choice = 0;
    int itemChoice;

    readCSV();

    getValidatedStringInput(location, sizeof(location), GREEN "✅ Please enter your delivery location: " RESET);

    while (true) {
        clearScreen();

        printf("\n" BLUE "Please select your restaurant:\n" RESET);
        printf("1) KFC\n2) McDonald's\n3) Pizza Hut\n4) View Previous Orders\n5) Exit\n");

        choice = getValidatedIntInput(1, 5, "Enter your choice (1-5): ");

        switch (choice) {
            case 1:
                printf(BLUE "\nKFC Menu:\n" RESET);
                printf("1) Marti Bucket\n2) Hot Booster\n");
                itemChoice = getValidatedIntInput(1, 2, "Enter your choice: ");

                snprintf(orders[orderCount].restaurant, sizeof(orders[orderCount].restaurant), "KFC");
                snprintf(orders[orderCount].item, sizeof(orders[orderCount].item), itemChoice == 1 ? "Marti Bucket" : "Hot Booster");
                snprintf(orders[orderCount].location, sizeof(orders[orderCount].location), "%s", location);
                orderCount++;

                printf(GREEN "\nYour order will be delivered to: %s\n" RESET, location);
                break;

            case 2:
                printf(BLUE "\nMcDonald's Menu:\n" RESET);
                printf("1) Big Mac\n2) Fries\n3) Happy Meal\n");
                itemChoice = getValidatedIntInput(1, 3, "Enter your choice: ");

                snprintf(orders[orderCount].restaurant, sizeof(orders[orderCount].restaurant), "McDonald's");
                snprintf(orders[orderCount].item, sizeof(orders[orderCount].item),
                         itemChoice == 1 ? "Big Mac" : itemChoice == 2 ? "Fries" : "Happy Meal");
                snprintf(orders[orderCount].location, sizeof(orders[orderCount].location), "%s", location);
                orderCount++;

                printf(GREEN "\nYour order will be delivered to: %s\n" RESET, location);
                break;

            case 3:
                printf(BLUE "\nPizza Hut Menu:\n" RESET);
                printf("1) Prosciutto Funghi pizza\n2) Custom Pizza\n");
                itemChoice = getValidatedIntInput(1, 2, "Enter your choice: ");

                if (itemChoice == 1) {
                    snprintf(orders[orderCount].restaurant, sizeof(orders[orderCount].restaurant), "Pizza Hut");
                    snprintf(orders[orderCount].item, sizeof(orders[orderCount].item), "Prosciutto Funghi pizza");
                } else {
                    int sizeChoice = getValidatedIntInput(1, 2, "Choose size: 1) Large 2) Small: ");
                    printf("Choose up to 2 ingredients:\n1) Pepperoni\n2) Olives\n3) Mushrooms\n");

                    int ing1 = getValidatedIntInput(1, 3, "First ingredient: ");
                    int ing2 = getValidatedIntInput(0, 3, "Second ingredient (0 to skip): ");

                    snprintf(orders[orderCount].restaurant, sizeof(orders[orderCount].restaurant), "Pizza Hut");
                    snprintf(orders[orderCount].item, sizeof(orders[orderCount].item),
                             sizeChoice == 1 ? "Large Custom Pizza" : "Small Custom Pizza");
                }
                snprintf(orders[orderCount].location, sizeof(orders[orderCount].location), "%s", location);
                orderCount++;

                printf(GREEN "\nYour order will be delivered to: %s\n" RESET, location);
                break;

            case 4:
                displayOrders();
                break;

            case 5:
                printf(GREEN "Exiting the program.\n" RESET);
                createCSV();
                return 0;
        }

        int continueChoice = getValidatedIntInput(1, 2, "\nDo you want to make another order?\n1) Yes\n2) No\n");
        if (continueChoice == 2) {
            printf(GREEN "Thank you for your orders. Exiting.\n" RESET);
            createCSV();
            break;
        }
        clearScreen();
    }

    return 0;
}