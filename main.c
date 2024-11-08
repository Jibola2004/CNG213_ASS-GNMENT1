#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

// Function to initialize an empty list
void MakeEmptyList(struct ListRecord *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Function to check if the list is empty
int IsEmptyList(struct ListRecord *list) {
    return list->size == 0;
}

// Function to update nutritional status based on BMI
void UpdateNutritionalStatus(struct Node* newNode) {
    float BMI = (newNode->weight / pow(newNode->height, 2));

    if (BMI < 18.5) {
        strcpy(newNode->nutritionStatus, "Underweight");
    } else if (BMI < 24.9) {
        strcpy(newNode->nutritionStatus, "Normal Weight");
    } else if (BMI < 29.9) {
        strcpy(newNode->nutritionStatus, "Pre-obesity");
    } else if (BMI < 34.9) {
        strcpy(newNode->nutritionStatus, "Obese class I");
    } else if (BMI < 39.9) {
        strcpy(newNode->nutritionStatus, "Obese class II");
    } else {
        strcpy(newNode->nutritionStatus, "Obese class III");
    }
}

// Function to read and sort the patients from the file
struct ListRecord* initialisePatients(char* filename) {
    FILE *infile = fopen(filename, "r");
    if (infile == NULL) {
        printf("File not found.\n");
        return NULL;  // Return NULL if file cannot be opened
    }

    struct ListRecord *list = (struct ListRecord *)malloc(sizeof(struct ListRecord));
    if (list == NULL) {
        printf("Out of memory!\n");
        fclose(infile); // Close the file before returning
        return NULL;
    }

    MakeEmptyList(list);  // Initialize list

    while (1) {
        struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
        if (newNode == NULL) {
            printf("Out of memory!\n");
            fclose(infile);
            return NULL;  // Return NULL if memory allocation fails
        }

        // Read patient data
        if (fscanf(infile, "%20[^;];%20[^;];%20[^;];%f;%f;%30s\n",
                   newNode->firstname, newNode->lastname,
                   newNode->DOB, &newNode->height, &newNode->weight,
                   newNode->nutritionStatus) != 6) {
            free(newNode);  // Free memory if read fails
            break;  // Exit the loop if not enough data is read
        }
        UpdateNutritionalStatus(newNode);  // Update the nutritional status after reading data

        newNode->next = NULL;  // Initialize the next pointer

        // Insert the node in sorted order by first name
        if (IsEmptyList(list)) {
            list->head = newNode;  // Set head to the new node if list is empty
            list->tail = newNode;  // Also set tail to the new node
        } else {
            struct Node* current = list->head;
            struct Node* previous = NULL;

            // Find the correct position to insert the node
            while (current != NULL && strcmp(current->firstname, newNode->firstname) < 0) {
                previous = current;
                current = current->next;
            }

            // Insert at the beginning of the list
            if (previous == NULL) {
                newNode->next = list->head;
                list->head = newNode;
            } else {
                // Insert in the middle or at the end
                previous->next = newNode;
                newNode->next = current;
            }

            // Update tail pointer if inserted at the end
            if (newNode->next == NULL) {
                list->tail = newNode;
            }
        }
        list->size++;  // Increment the list size
    }

    fclose(infile);  // Close the file
    return list;  // Return the populated and sorted list
}

// Function to print the list of patients
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

// Function to free the list and its nodes
void freeList(struct ListRecord* list) {
    struct Node* current = list->head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);  // Free the list structure itself
}

// Function to insert a new patient into the list
void InsertNewPatient(struct ListRecord* list) {
    if (list == NULL) {
        printf("The list does not exist, creating an empty list.\n");
        MakeEmptyList(list);
    }

    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Out of memory!\n");
        exit(1);
    }

    // Prompt the user for patient data
    printf("Enter new patient firstname: ");
    scanf("%s", newNode->firstname);
    printf("Enter new patient lastname: ");
    scanf("%s", newNode->lastname);
    printf("Enter new patient date of birth (dd/mm/yyyy): ");
    scanf("%s", newNode->DOB);
    printf("Enter new patient height (m): ");
    scanf("%f", &newNode->height);
    printf("Enter new patient weight (kg): ");
    scanf("%f", &newNode->weight);

    UpdateNutritionalStatus(newNode);

    newNode->next = NULL;  // Initialize the next pointer

    // Insert the node in sorted order
    if (IsEmptyList(list)) {
        list->head = newNode;  // Set head to the new node if the list is empty
        list->tail = newNode;  // Set tail to the new node
    } else {
        struct Node* current = list->head;
        struct Node* previous = NULL;

        // Find the correct position to insert the new node
        while (current != NULL && strcmp(current->firstname, newNode->firstname) < 0) {
            previous = current;
            current = current->next;
        }

        // Insert at the beginning of the list
        if (previous == NULL) {
            newNode->next = list->head;
            list->head = newNode;
        } else {
            // Insert in the middle or at the end
            previous->next = newNode;
            newNode->next = current;
        }

        // Update the tail pointer if inserted at the end
        if (newNode->next == NULL) {
            list->tail = newNode;
        }
    }

    list->size++;  // Increment the size
    printf("New patient has been added successfully.\n");
}

// Function to search for a patient by first name
void SearchPatient(struct ListRecord* list, char searchName[]) {
    struct Node *tmp = list->head;
    int found = 0;

    while (tmp != NULL) {
        // Check if the current patient's first name matches the search query
        if (strcmp(searchName, tmp->firstname) == 0) {
            found = 1;
            printf("Patient found:\n");
            printf("%s;%s;%s;%.2f;%.2f;%s\n",
                   tmp->firstname, tmp->lastname,
                   tmp->DOB, tmp->height, tmp->weight,
                   tmp->nutritionStatus);
        }
        tmp = tmp->next;  // Move to the next node in the list
    }

    if (!found) {
        printf("Patient not found in the list.\n");
    }
}

// Function to initialize the unhealthy patient list
void initialiseUnhealthyPatientList(struct ListRecord* list, struct ListRecord* unhealthylist) {
    if (unhealthylist == NULL) {
        printf("The unhealthy list does not exist, creating an empty list.\n");
        MakeEmptyList(unhealthylist); // Initialize unhealthy list if it's NULL
    }

    struct Node *patient = list->head;
    while (patient != NULL) {
        // Check if the patient's nutrition status indicates obesity
        if (strcmp("Obese class I", patient->nutritionStatus) == 0 ||
            strcmp("Obese class II", patient->nutritionStatus) == 0 ||
            strcmp("Obese class III", patient->nutritionStatus) == 0) {

            struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
            if (newNode == NULL) {
                printf("Out of memory!\n");
                exit(1); // Exit if memory allocation fails
            }

            // Copy the patient information to the new node
            strcpy(newNode->firstname, patient->firstname);
            strcpy(newNode->lastname, patient->lastname);
            strcpy(newNode->DOB, patient->DOB);
            strcpy(newNode->nutritionStatus, patient->nutritionStatus);
            newNode->height = patient->height;
            newNode->weight = patient->weight;

            newNode->next = NULL;

            // Add new node to unhealthy list
            if (IsEmptyList(unhealthylist)) {
                unhealthylist->head = newNode;
                unhealthylist->tail = newNode;
            } else {
                unhealthylist->tail->next = newNode;
                unhealthylist->tail = newNode;
            }

            unhealthylist->size++;
        }

        patient = patient->next; // Move to the next patient
    }
}

// Main function
int main(int argc, char *argv[]) {
    char *filename;
    char searchName[20];

    if (argc > 1) {
        filename = argv[1];  // Get the filename from the command-line argument
    } else {
        // Prompt user to enter the filename
        char inputFilename[50];
        printf("Enter the filename: ");
        scanf("%s", inputFilename);
        filename = inputFilename;
    }

    struct ListRecord* UnhealthyPatientList = (struct ListRecord *) malloc(sizeof(struct ListRecord));
    struct ListRecord* patientList = initialisePatients((char *)filename);

    if (patientList != NULL) {
        printf("Patients in sorted order by first name:\n");
        printPatients(patientList);   // Print the list of patients for verification

        // Initialize the unhealthy patient list
        //initialiseUnhealthyPatientList(patientList, UnhealthyPatientList);
       // printf("Unhealthy Patients List:\n");
      //  printPatients(UnhealthyPatientList);

        // Clean up memory
        freeList(patientList);
        freeList(UnhealthyPatientList);  // Free the unhealthy list as well
    } else {
        printf("Failed to initialize patient list.\n");
    }

    return 0;
}

