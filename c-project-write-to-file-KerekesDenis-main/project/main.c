#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Color codes
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define MAX_ORDERS 100

typedef struct {
    char restaurant[50];
    char item[50];
    char location[100];
} Order;

Order orders[MAX_ORDERS];
int orderCount = 0;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int getIntInput() {
    int val;
    while (true) {
        int result = scanf("%d", &val);
        if (result == 1) {
            clearInputBuffer();
            return val;
        } else {
            printf(RED "Invalid input. Please enter a number: " RESET);
            clearInputBuffer();
        }
    }
}

void createCSV() {
    char filePath[] = "C:\\Users\\bazinga\\Desktop\\c-project-write-to-file-KerekesDenis-main\\orders.txt";
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
    char filePath[] = "C:\\Users\\bazinga\\Desktop\\c-project-write-to-file-KerekesDenis-main\\orders.txt";
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        printf(YELLOW "Error opening file for reading. If it's the first time, no previous orders.\n" RESET);
        return;
    }

    char buffer[200];
    fgets(buffer, sizeof(buffer), file); // Skip header

    while (fgets(buffer, sizeof(buffer), file) && orderCount < MAX_ORDERS) {
        Order newOrder;
        sscanf(buffer, "%49[^,],%49[^,],%99[^\n]", newOrder.restaurant, newOrder.item, newOrder.location);
        orders[orderCount++] = newOrder;
    }

    fclose(file);
    printf(GREEN "Previous orders loaded successfully.\n" RESET);
}

void displayOrders() {
    if (orderCount == 0) {
        printf("\n" YELLOW "No previous orders found.\n" RESET);
    } else {
        printf("\n" CYAN "Previous Orders:\n" RESET);
        printf("Restaurant, Item, Location\n");
        for (int i = 0; i < orderCount; i++) {
            printf(BLUE "%s" RESET ", " MAGENTA "%s" RESET ", " GREEN "%s" RESET "\n",
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

int main() {
    char location[100];
    int choice = 0;
    int itemChoice;

    readCSV();

    printf("Please enter your location: ");
    // Use fgets to allow spaces in location
    if (fgets(location, sizeof(location), stdin) != NULL) {
        // Remove newline at end if present
        size_t len = strlen(location);
        if (len > 0 && location[len - 1] == '\n') {
            location[len - 1] = '\0';
        }
    }

    while (true) {
        clearScreen();

        printf("\nPlease select your restaurant\n");
        printf("1) " RED "KFC" RESET "\n");
        printf("2) " YELLOW "McDonald's" RESET "\n");
        printf("3) " GREEN "Pizza Hut" RESET "\n");
        printf("4) View Previous Orders\n");
        printf("5) Exit\n");

        printf("Enter your choice: ");
        choice = getIntInput();

        switch (choice) {
            case 1:
                printf("\nYou selected " RED "KFC" RESET ", here's the menu:\n");
                printf("1) " MAGENTA "Marti Bucket" RESET "\n");
                printf("2) " MAGENTA "Hot Booster" RESET "\n");
                printf("Enter your choice: ");
                itemChoice = getIntInput();

                if (itemChoice == 1 || itemChoice == 2) {
                    if (orderCount >= MAX_ORDERS) {
                        printf(RED "Order limit reached. Cannot add more orders.\n" RESET);
                        break;
                    }

                    snprintf(orders[orderCount].restaurant, sizeof(orders[orderCount].restaurant), "KFC");
                    snprintf(orders[orderCount].item, sizeof(orders[orderCount].item), itemChoice == 1 ? "Marti Bucket" : "Hot Booster");
                    snprintf(orders[orderCount].location, sizeof(orders[orderCount].location), "%s", location);
                    orderCount++;

                    printf("You selected %s.\n", itemChoice == 1 ? "Marti Bucket" : "Hot Booster");
                    printf("\nYour order will be delivered to your address: " CYAN "%s.\n" RESET, location);
                } else {
                    printf(RED "Invalid choice for KFC.\n" RESET);
                }
                break;

            case 2:
                printf("\nYou selected " YELLOW "McDonald's" RESET ", here's the menu:\n");
                printf("1) " MAGENTA "Big Mac" RESET "\n");
                printf("2) " MAGENTA "Fries" RESET "\n");
                printf("3) " MAGENTA "Happy Meal" RESET "\n");
                printf("Enter your choice: ");
                itemChoice = getIntInput();

                if (itemChoice >= 1 && itemChoice <= 3) {
                    if (orderCount >= MAX_ORDERS) {
                        printf(RED "Order limit reached. Cannot add more orders.\n" RESET);
                        break;
                    }

                    snprintf(orders[orderCount].restaurant, sizeof(orders[orderCount].restaurant), "McDonald's");

                    if (itemChoice == 1)
                        snprintf(orders[orderCount].item, sizeof(orders[orderCount].item), "Big Mac");
                    else if (itemChoice == 2)
                        snprintf(orders[orderCount].item, sizeof(orders[orderCount].item), "Fries");
                    else
                        snprintf(orders[orderCount].item, sizeof(orders[orderCount].item), "Happy Meal");

                    snprintf(orders[orderCount].location, sizeof(orders[orderCount].location), "%s", location);
                    orderCount++;

                    printf("\nYour order will be delivered to your address: " CYAN "%s.\n" RESET, location);
                } else {
                    printf(RED "Invalid choice for McDonald's.\n" RESET);
                }
                break;

            case 3:
                printf("\nYou selected " GREEN "Pizza Hut" RESET ", here's the menu:\n");
                printf("1) " MAGENTA "Prosciutto Funghi pizza" RESET "\n");
                printf("2) " MAGENTA "Custom Pizza" RESET "\n");
                printf("Enter your choice: ");
                itemChoice = getIntInput();

                if (itemChoice == 1) {
                    if (orderCount >= MAX_ORDERS) {
                        printf(RED "Order limit reached. Cannot add more orders.\n" RESET);
                        break;
                    }

                    snprintf(orders[orderCount].restaurant, sizeof(orders[orderCount].restaurant), "Pizza Hut");
                    snprintf(orders[orderCount].item, sizeof(orders[orderCount].item), "Prosciutto Funghi pizza");
                    snprintf(orders[orderCount].location, sizeof(orders[orderCount].location), "%s", location);
                    orderCount++;

                    printf("\nYour order will be delivered to your address: " CYAN "%s.\n" RESET, location);
                } else if (itemChoice == 2) {
                    printf("\nPlease select the size:\n");
                    printf("1) Large\n");
                    printf("2) Small\n");
                    printf("Enter your choice: ");
                    int sizeChoice = getIntInput();

                    if (sizeChoice == 1 || sizeChoice == 2) {
                        printf("\nYou selected a %s pizza. Please select up to two ingredients:\n", sizeChoice == 1 ? "large" : "small");
                        printf("1) Pepperoni\n");
                        printf("2) Olives\n");
                        printf("3) Mushrooms\n");

                        int ingredientChoice1 = 0, ingredientChoice2 = 0;
                        int ingredientCount = 0;

                        printf("Enter your first ingredient choice: ");
                        ingredientChoice1 = getIntInput();

                        if (ingredientChoice1 >= 1 && ingredientChoice1 <= 3) {
                            ingredientCount++;
                        } else {
                            printf(RED "Invalid first ingredient choice.\n" RESET);
                        }

                        if (ingredientCount < 2) {
                            printf("Enter your second ingredient choice (or 0 to skip): ");
                            ingredientChoice2 = getIntInput();

                            if ((ingredientChoice2 >= 1 && ingredientChoice2 <= 3) && (ingredientChoice2 != ingredientChoice1)) {
                                ingredientCount++;
                            } else if (ingredientChoice2 == 0) {
                                printf("No second ingredient selected.\n");
                            } else {
                                printf(RED "Invalid second ingredient choice.\n" RESET);
                            }
                        }

                        if (orderCount >= MAX_ORDERS) {
                            printf(RED "Order limit reached. Cannot add more orders.\n" RESET);
                            break;
                        }

                        snprintf(orders[orderCount].restaurant, sizeof(orders[orderCount].restaurant), "Pizza Hut");
                        snprintf(orders[orderCount].item, sizeof(orders[orderCount].item),
                                 sizeChoice == 1 ? "Large Custom Pizza" : "Small Custom Pizza");
                        snprintf(orders[orderCount].location, sizeof(orders[orderCount].location), "%s", location);
                        orderCount++;

                        printf("Your custom pizza will be delivered to your address: " CYAN "%s.\n" RESET, location);
                    } else {
                        printf(RED "Invalid size choice.\n" RESET);
                    }
                } else {
                    printf(RED "Invalid choice for Pizza Hut.\n" RESET);
                }
                break;

            case 4:
                displayOrders();
                break;

            case 5:
                printf(GREEN "Exiting the program.\n" RESET);
                createCSV();
                return 0;

            default:
                printf(RED "Invalid choice, please try again.\n" RESET);
                break;
        }

        printf("\nDo you want to make another order?\n");
        printf("1) Yes\n");
        printf("2) No\n");
        printf("Enter your choice: ");
        int continueChoice = getIntInput();

        if (continueChoice == 2) {
            printf(GREEN "Thank you for your orders. Exiting the program.\n" RESET);
            createCSV();
            break;
        }
        clearScreen();
    }

    return 0;
}
