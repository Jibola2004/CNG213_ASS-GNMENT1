#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char firstname[20];
    char lastname[20];
    char DOB[20];
    float height;
    float weight;
    char nutritionStatus[30];
    struct Node *next;
};

struct ListRecord {
    struct Node* head;
    struct Node* tail;
    int size;
};

void MakeEmptyList(struct ListRecord *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

int IsEmptyList(struct ListRecord *list) {
    return (list->size == 0);
}

struct ListRecord* initialisePatients(char* filename) {
    FILE *infile = fopen(filename, "r");
    if (infile == NULL) {
        printf("File not found.\n");
        exit(1);
    }

    struct ListRecord *list = (struct ListRecord *) malloc(sizeof(struct ListRecord));
    if (list == NULL) {
        printf("Out of memory!\n");
        exit(1);
    }

    MakeEmptyList(list); // Initialize the list

    while (1) {
        struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
        if (newNode == NULL) {
            printf("Out of memory!\n");
            exit(1);
        }

        // Read patient data
        if (fscanf(infile, "%20[^;];%20[^;];%20[^;];%f;%f;%30s\n",
                   newNode->firstname, newNode->lastname,
                   newNode->DOB, &newNode->height, &newNode->weight,
                   newNode->nutritionStatus) != 6) {
            free(newNode); // Free memory if read fails
            break; // Exit the loop if not enough data
        }

        newNode->next = NULL; // Initialize the next pointer
        list->tail = newNode; // Update tail to the new node
        list->size++; // Increment size

        // Insert the node in sorted order
        if (IsEmptyList(list)) {
            list->head = newNode; // Set head to the new node if list is empty
        } else {
            struct Node* current = list->head;
            struct Node* previous = NULL;

            // Find the correct position to insert
            while (current != NULL && strcmp(current->firstname, newNode->firstname) < 0) {
                previous = current;
                current = current->next;
            }

            // Insert at the beginning
            if (previous == NULL) {
                newNode->next = list->head;
                list->head = newNode;
            } else {
                // Insert in the middle or end
                previous->next = newNode;
                newNode->next = current;
            }
        }
    }

    fclose(infile); // Close the file
    return list; // Return the populated list
}

void freeList(struct ListRecord* list) {
    struct Node* current = list->head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(list); // Free the list structure itself
}

void printPatients(struct ListRecord* list) {
    struct Node* current = list->head;
    while (current != NULL) {
        printf("%s;%s;%s;%.2f;%.2f;%s\n",
               current->firstname, current->lastname,
               current->DOB, current->height, current->weight,
               current->nutritionStatus);
        current = current->next;
    }
}

int main(int argc, char *argv[]) {
    const char *filename;

    if (argc > 1) {
        filename = argv[1]; // Get filename from command line argument
    } else {
        // Prompt user to enter the filename
        char inputFilename[50];
        printf("Enter the filename: ");
        scanf("%49s", inputFilename);
        filename = inputFilename;
    }

    struct ListRecord* patientList = initialisePatients((char *)filename);
    printf("Patients in sorted order by first name:\n");
    printPatients(patientList); // Print the patients for verification

    freeList(patientList); // Clean up memory
    return 0;
}
