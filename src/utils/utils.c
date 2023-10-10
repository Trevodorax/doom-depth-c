#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"

char* custom_itoa(int value, char* str, int base) {
    sprintf(str, (base == 16) ? "%x" : (base == 8) ? "%o" : "%d", value);
    return str;
}

char* custom_strupr(char* str) {
    if (str == NULL) {
        return NULL;
    }

    char * new_str = strdup(str);

    for (size_t i = 0; new_str[i]; i++) {
        new_str[i] = toupper((unsigned char)str[0]);
    }
    return new_str;
}

void str_split_lines(array_node_t **head_ref, const char *string) {
    char *temp_string = strdup(string);
    char *token = strtok(temp_string, "\n");

    while (token != NULL) {
        append(head_ref, token, sizeof(char) * (strlen(token) + 1));
        token = strtok(NULL, "\n");
    }
}