#include "array.h"
#include "../entities/player/player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @brief Creates a linked list.
 *
 * This function creates a linked list.
 *
 * @param type The type of the structures that will be contained in the list.
 * @return Pointer to the head of the linked list.
 */
list_t * new_list(array_type_t type) {
    list_t *list = malloc(sizeof(list_t));
    if(list == NULL) {
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->size = 0;
    list->type_size = get_size_of_type(type);
    return list;
}

/**
 * @brief Adds an element to the beginning of the linked list.
 *
 * This function adds a new node at the beginning of the linked list and copies the value into it.
 *
 * @param list Pointer to the head of the linked list.
 * @param new_data Pointer to the value to be added.
 * @sideeffects Modifies the linked list by adding a new node at the beginning.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void push(list_t *list, void *new_data) {
    array_node_t *new_node = (array_node_t *)malloc(sizeof(array_node_t));

    new_node->value = malloc(list->type_size);
    new_node->next = list->head;

    memcpy(new_node->value, new_data, list->type_size);
    list->head = new_node;

    list->size++;
}

/**
 * @brief Adds an element to the end of the linked list.
 *
 * This function adds a new node at the end of the linked list and copies the value into it.
 *
 * @param head_ref Pointer to the linked list.
 * @param new_data Pointer to the value to be added.
 * @sideeffects Modifies the linked list by adding a new node at the end.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void append(list_t *list, void *new_data) {
    array_node_t *new_node = (array_node_t *)malloc(sizeof(array_node_t));
    array_node_t *last = list->head;

    new_node->value = malloc(list->type_size);
    new_node->next = NULL;

    if(new_data) {
        memcpy(new_node->value, new_data, list->type_size);
    }

    if (list->head == NULL) {
        list->head = new_node;
        list->size++;

        return;
    }

    while (last->next != NULL) {
        last = last->next;
    }

    last->next = new_node;
    list->size++;
}

void remove_node(list_t *list, void **data_to_remove) {
    if (list->head == NULL) {
        return;
    }

    array_node_t *temp = list->head;

    if (temp->value == *data_to_remove) {
        list->head = temp->next;
        free(temp->value);
        free(temp);
        list->size--;
        return;
    }

    while (temp->next != NULL && temp->next->value != *data_to_remove) {
        temp = temp->next;
    }

    if (temp->next == NULL) {
        return;
    }

    array_node_t *next = temp->next->next;
    free(temp->next->value);
    free(temp->next);
    temp->next = next;
    list->size--;
}

/**
 * @brief Deletes an element at a given index.
 *
 * This function deletes a node at a given index in the linked list.
 *
 * @param list Pointer to the linked list.
 * @param index The index of the node to be deleted.
 * @sideeffects Modifies the linked list by removing a node.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void delete_node(list_t *list, int index) {
    if (list->head == NULL || index < 0) {
        return;
    }

    array_node_t *temp = list->head;

    if (index == 0) {
        list->head = temp->next;
        free(temp->value);
        free(temp);
        list->size--;
        return;
    }

    for (int i = 0; temp != NULL && i < index - 1; i++) {
        temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL) {
        return;
    }

    array_node_t *next = temp->next->next;
    free(temp->next->value);
    free(temp->next);
    temp->next = next;
    list->size--;
}

/**
 * @brief Inserts an element at a given index in the linked list.
 *
 * This function inserts a new node at a specified index in the linked list and copies the value into it.
 *
 * @param head_ref Pointer to the linked list.
 * @param new_data Pointer to the value to be added.
 * @param index The index at which the new node should be inserted.
 * @return 0 if the operation is successful, -1 otherwise.
 * @sideeffects Modifies the linked list by adding a new node at the specified index.
 * @dependencies No dependencies.
 * @errors Returns -1 if the index is out of bounds.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
int insert_at_index(list_t *list, void *new_data, int index) {
    if (index < 0) {
        return -1;  // Index out of bounds
    }

    array_node_t *new_node = (array_node_t *)malloc(sizeof(array_node_t));
    new_node->value = malloc(list->type_size);
    memcpy(new_node->value, new_data, list->type_size);

    if (index == 0) {
        new_node->next = list->head;
        list->head = new_node;

        list->size++;
        return 0;
    }

    array_node_t *prev = list->head;
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

    list->size++;

    return 0;
}

/**
 * @brief Finds an element in the linked list.
 *
 * This function searches for an element in the linked list and returns its index.
 *
 * @param head Pointer to the linked list.
 * @param data_to_find Pointer to the value to find.
 * @return The index of the found node, or -1 if not found.
 * @sideeffects No side effects.
 * @dependencies No dependencies.
 * @errors Returns -1 if the element is not found.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
int find_node(list_t *list, void *data_to_find) {
    array_node_t *current = list->head;
    int index = 0;

    if(current == NULL) {
        return -1;
    }

    while (current != NULL) {
        // Compare memory block
        if (memcmp(current->value, data_to_find, list->type_size) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    return -1;
}

/**
 * @brief Retrieves the node at a given index in the linked list.
 *
 * This function traverses the linked list to find the node at the specified index.
 * The index is 0-based, meaning that passing 0 will return the first node, 1 will return the second, and so on.
 *
 * @param head Pointer to the linked list.
 * @param index The 0-based index of the node to be retrieved.
 * @return Pointer to the node at the specified index, or NULL if the index is out of bounds.
 * @sideeffects No side effects.
 * @errors Returns NULL if the index is out of bounds.
 * @author Noam DE MASURE
 * @date 27/09/2023
 */
void * get_value_at_index(list_t *list, int index) {
    array_node_t *current = list->head;

    if(current == NULL){
        return NULL;
    }
    while (index != 0){
        if (current->next!=NULL){
            current = current->next;
        } else {
            return NULL;
        }
        index--;
    }

    return current->value;
}

/**
 * @brief Returns the size of a given type.
 *
 * This function returns the size of a given type.
 *
 * @param type The type of which the size is to be returned.
 * @return The size of the type.
 * @sideeffects No side effects.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 */
size_t get_size_of_type(array_type_t type) {
    switch(type) {
        case PLAYER_STRUCT:
            return sizeof(player_t);
        case MONSTER_STRUCT:
            return sizeof(monster_t);
        case WEAPON_STRUCT:
            return sizeof(weapon_t);
        case ARMOR_STRUCT:
            return sizeof(armor_t);
        case SPELL_STRUCT:
            return sizeof(spell_t);
        case INT_STRUCT:
            return sizeof(int);
        case STRING_STRUCT:
            return sizeof(char*);
        case FLOAT_STRUCT:
            return sizeof(float);
        case INVENTORY_STRUCT:
            return sizeof(inventory_t);
        default:
            return 0;
    }
}

/**
 * @brief Frees the linked list.
 *
 * This function frees all the nodes in the linked list and sets the head to NULL, free the list after.
 *
 * @param head_ref Pointer to the linked list.
 * @sideeffects Frees the memory allocated for the linked list.
 * @dependencies No dependencies.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void free_list(list_t *list) {
    array_node_t *current = list->head;
    array_node_t *next;

    while (current != NULL) {
        next = current->next;
        // TODOSOMEDAY: add an enum with the type of entity to free accordingly
        free(current->value);
        free(current);
        current = next;
    }

    list->head = NULL;
    free(list);
}

void free_array_node(array_node_t *node) {
    if (node) {
        free(node->value);
        free(node);
    }
}

/**
 * @brief Prints the linked list.
 *
 * This function prints the elements of the linked list using a provided print function.
 *
 * @param node Pointer to the linked list.
 * @param fptr Pointer to the function to print the value.
 * @sideeffects Prints the elements of the linked list to stdout.
 * @dependencies Depends on the provided print function.
 * @errors No error handling implemented.
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void print_list(list_t *list, void (*fptr)(void *)) {
    array_node_t *node = list->head;
    while (node != NULL) {
        (*fptr)(node->value);
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
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void print_string(void *str) {
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
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void print_int(void *n) {
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
 * @author Noam DE MASURE
 * @date 26/09/2023
 */
void print_float(void *f) {
    printf(" %f", *(float *)f);
}