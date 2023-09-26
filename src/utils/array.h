#ifndef DOOM_DEPTH_C_ARRAY_H
#define DOOM_DEPTH_C_ARRAY_H
typedef struct ArrayNode {
    void *value;
    struct ArrayNode *next;
} ArrayNode;

void push(ArrayNode **head_ref, void *new_data, size_t data_size);
void append(ArrayNode **head_ref, void *new_data, size_t data_size);
void deleteNode(ArrayNode **head_ref, int index);
int findNode(ArrayNode *head, void *data_to_find, size_t data_size);
void freeList(ArrayNode **head_ref);
void printList(ArrayNode *node, void (*fptr)(void *));

#endif //DOOM_DEPTH_C_ARRAY_H
