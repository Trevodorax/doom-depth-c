#ifndef DOOM_DEPTH_C_ARRAY_H
#define DOOM_DEPTH_C_ARRAY_H

#include <stdio.h>

typedef enum array_type_t {
    INVENTORY_STRUCT,
    PLAYER_STRUCT,
    MONSTER_STRUCT,
    WEAPON_STRUCT,
    ARMOR_STRUCT,
    SPELL_STRUCT,
    INT_STRUCT,
    STRING_STRUCT,
    FLOAT_STRUCT
} array_type_t;

typedef struct array_node_t {
    void *value;
    struct array_node_t *next;
} array_node_t;

typedef struct array_list_t {
    array_node_t *head;
    int size;
    size_t type_size;
} list_t;

list_t * new_list(array_type_t type);
void push(list_t *list, void *new_data);
void append(list_t *list, void *new_data);
void delete_node(list_t *list, int index);
void remove_node(list_t *list, void **data_to_remove);
int find_node(list_t *list, void *data_to_find);
void * get_value_at_index(list_t *list,  int index);
int get_size(array_node_t *node);
void free_list(list_t *list);
void print_list(list_t *list, void (*fptr)(void *));
void print_string(void *str);
void print_int(void *n);
void print_float(void *f);
size_t get_size_of_type(array_type_t type);

/**
 * @brief Frees the array node NOT recursively and WITHOUT the value
 * @param node The array node to free
 */
void free_array_node(array_node_t *node);

/**
 * @brief Returns the number of elements in a linked list
 *
 * @param head Pointer to the first element of the linked list
 *
 */
int get_count(array_node_t *head);

#endif //DOOM_DEPTH_C_ARRAY_H
