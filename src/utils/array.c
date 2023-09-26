#include "array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Adds an element to the beginning of the linked list.
 *
 * This function adds a new node at the beginning of the linked list and copies the value into it.
 *
 * @param head_ref Pointer to the head of the linked list.
 * @param new_data Pointer to the value to be added.
 * @param data_size Size of the value type.
 * @sideeffects Modifies the linked list by adding a new node at the beginning.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 */
void push(ArrayNode **head_ref, void *new_data, size_t data_size) {
    ArrayNode *new_node = (ArrayNode *)malloc(sizeof(ArrayNode));
    new_node->value = malloc(data_size);
    new_node->next = *head_ref;

    memcpy(new_node->value, new_data, data_size);
    *head_ref = new_node;
}

/**
 * @brief Adds an element to the end of the linked list.
 *
 * This function adds a new node at the end of the linked list and copies the value into it.
 *
 * @param head_ref Pointer to the head of the linked list.
 * @param new_data Pointer to the value to be added.
 * @param data_size Size of the value type.
 * @sideeffects Modifies the linked list by adding a new node at the end.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 */
void append(ArrayNode **head_ref, void *new_data, size_t data_size) {
    ArrayNode *new_node = (ArrayNode *)malloc(sizeof(ArrayNode));
    ArrayNode *last = *head_ref;
    new_node->value = malloc(data_size);
    new_node->next = NULL;

    memcpy(new_node->value, new_data, data_size);

    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    while (last->next != NULL) {
        last = last->next;
    }

    last->next = new_node;
}

/**
 * @brief Deletes an element at a given index.
 *
 * This function deletes a node at a given index in the linked list.
 *
 * @param head_ref Pointer to the head of the linked list.
 * @param index The index of the node to be deleted.
 * @sideeffects Modifies the linked list by removing a node.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 */
void deleteNode(ArrayNode **head_ref, int index) {
    if (*head_ref == NULL || index < 0) {
        return;
    }

    ArrayNode *temp = *head_ref;

    if (index == 0) {
        *head_ref = temp->next;
        free(temp->value);
        free(temp);
        return;
    }

    for (int i = 0; temp != NULL && i < index - 1; i++) {
        temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL) {
        return;
    }

    ArrayNode *next = temp->next->next;
    free(temp->next->value);
    free(temp->next);
    temp->next = next;
}

/**
 * @brief Inserts an element at a given index in the linked list.
 *
 * This function inserts a new node at a specified index in the linked list and copies the value into it.
 *
 * @param head_ref Pointer to the head of the linked list.
 * @param new_data Pointer to the value to be added.
 * @param data_size Size of the value type.
 * @param index The index at which the new node should be inserted.
 * @return 0 if the operation is successful, -1 otherwise.
 * @sideeffects Modifies the linked list by adding a new node at the specified index.
 * @dependencies No dependencies.
 * @errors Returns -1 if the index is out of bounds.
 */
int insertAtIndex(ArrayNode **head_ref, void *new_data, size_t data_size, int index) {
    if (index < 0) {
        return -1;  // Index out of bounds
    }

    ArrayNode *new_node = (ArrayNode *)malloc(sizeof(ArrayNode));
    new_node->value = malloc(data_size);
    memcpy(new_node->value, new_data, data_size);

    if (index == 0) {
        new_node->next = *head_ref;
        *head_ref = new_node;
        return 0;
    }

    ArrayNode *prev = *head_ref;
    for (int i = 0; i < index - 1; i++) {
        if (prev == NULL) {
            free(new_node->value);
            free(new_node);
            return -1;  // Index out of bounds
        }
        prev = prev->next;
    }

    new_node->next = prev->next;
    prev->next = new_node;

    return 0;
}

/**
 * @brief Finds an element in the linked list.
 *
 * This function searches for an element in the linked list and returns its index.
 *
 * @param head Pointer to the head of the linked list.
 * @param data_to_find Pointer to the value to find.
 * @param data_size Size of the value type.
 * @return The index of the found node, or -1 if not found.
 * @sideeffects No side effects.
 * @dependencies No dependencies.
 * @errors Returns -1 if the element is not found.
 */
int findNode(ArrayNode *head, void *data_to_find, size_t data_size) {
    ArrayNode *current = head;
    int index = 0;

    while (current != NULL) {
        // Compare memory block
        if (memcmp(current->value, data_to_find, data_size) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}

/**
 * @brief Frees the linked list.
 *
 * This function frees all the nodes in the linked list and sets the head to NULL.
 *
 * @param head_ref Pointer to the head of the linked list.
 * @sideeffects Frees the memory allocated for the linked list.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 */
void freeList(ArrayNode **head_ref) {
    ArrayNode *current = *head_ref;
    ArrayNode *next;

    while (current != NULL) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }

    *head_ref = NULL;
}


/**
 * @brief Prints the linked list.
 *
 * This function prints the elements of the linked list using a provided print function.
 *
 * @param node Pointer to the head of the linked list.
 * @param fptr Pointer to the function to print the value.
 * @sideeffects Prints the elements of the linked list to stdout.
 * @dependencies Depends on the provided print function.
 * @errors No error handling implemented.
 */
void printList(ArrayNode *node, void (*pointer_function)(void *)) {
    while (node != NULL) {
        (*pointer_function)(node->value);
        node = node->next;
    }
    printf("\n");
}

/**
 * @brief Prints a string.
 *
 * This function prints a string (char *) to the standard output.
 *
 * @param str Pointer to the string to be printed.
 * @sideeffects Prints the string to the standard output.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 */
void printString(void *str) {
    printf(" %s", (char *)str);
}

/**
 * @brief Prints an integer.
 *
 * This function prints an integer (int) to the standard output.
 *
 * @param n Pointer to the integer to be printed.
 * @sideeffects Prints the integer to the standard output.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 */
void printInt(void *n) {
    printf(" %d", *(int *)n);
}

/**
 * @brief Prints a floating-point number.
 *
 * This function prints a floating-point number (float) to the standard output.
 *
 * @param f Pointer to the float to be printed.
 * @sideeffects Prints the float to the standard output.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 */
void printFloat(void *f) {
    printf(" %f", *(float *)f);
}

int example() {
    ArrayNode *start = NULL;

    // Example with integers
    unsigned int_size = sizeof(int);
    int arr[] = {10, 20, 30, 40, 50}, i;
    for (i = 4; i >= 0; i--) {
        push(&start, &arr[i], int_size);
    }
    printf("Liste d'entiers :");
    printList(start, printInt);

    // Example with floats
    unsigned float_size = sizeof(float);
    start = NULL;
    float arr_f[] = {10.1f, 20.2f, 30.3f, 40.4f, 50.5f};
    for (i = 4; i >= 0; i--) {
        push(&start, &arr_f[i], float_size);
    }
    printf("Liste de flottants :");
    printList(start, printFloat);

    return 0;
}
