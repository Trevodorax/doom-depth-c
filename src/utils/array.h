#ifndef DOOM_DEPTH_C_ARRAY_H
#define DOOM_DEPTH_C_ARRAY_H

#include <stdio.h>

typedef struct array_node_t {
    void * value;
    struct array_node_t * next;
} array_node_t;

void push(array_node_t ** head_ref, void * new_data, size_t data_size);
void append(array_node_t ** head_ref, void * new_data, size_t data_size);
void delete_node(array_node_t ** head_ref, int index);
void remove_node(array_node_t ** head_ref, void ** data_to_remove);
int find_node(array_node_t * head, void * data_to_find, size_t data_size);
void * get_value_at_index(array_node_t * head, int index);
int get_size(array_node_t * node);
void free_list(array_node_t ** head_ref);
void print_list(array_node_t * node, void (*fptr)(void *));
void print_string(void * str);
void print_int(void * n);
void print_float(void * f);

/**
 * @brief Frees the array node NOT recursively and WITHOUT the value
 * @param node The array node to free
 */
void free_array_node(array_node_t * node);

/**
 * @brief Returns the number of elements in a linked list
 *
 * @param head Pointer to the first element of the linked list
 *
 */
int get_count(array_node_t * head);

#endif //DOOM_DEPTH_C_ARRAY_H
