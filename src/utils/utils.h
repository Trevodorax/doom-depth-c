#ifndef DOOM_DEPTH_C_UTILS_H
#define DOOM_DEPTH_C_UTILS_H

#define min(a, b) ((a) < (b) ? (a) : (b))

#include "array.h"

char* custom_itoa(int value, char* str, int base);

char* custom_strupr(char* str);

void str_split_lines(array_node_t **head_ref, const char *string);

#endif //DOOM_DEPTH_C_UTILS_H
