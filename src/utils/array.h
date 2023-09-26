#ifndef DOOM_DEPTH_C_ARRAY_H
#define DOOM_DEPTH_C_ARRAY_H
typedef struct ArrayNode {
    void *value;
    struct ArrayNode *next;
} ArrayNode;

void push(ArrayNode **head_ref, void *new_data, size_t data_size);
void append(ArrayNode **head_ref, void *new_data, size_t data_size);
void delete_node(ArrayNode **head_ref, int index);
int find_node(ArrayNode *head, void *data_to_find, size_t data_size);
void * get_value_at_index(ArrayNode *head, int index);
void free_list(ArrayNode **head_ref);
void print_list(ArrayNode *node, void (*fptr)(void *));
void print_string(void *str);
void print_int(void *n);
void print_float(void *f);
#endif //DOOM_DEPTH_C_ARRAY_H
