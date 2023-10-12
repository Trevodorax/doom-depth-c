#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "json.h"

/**
 * @brief parses a json object
 *
 * @sideeffects moves the pointer to string to the end of the parsed object
 * @param json_string_ptr the string of the json object
 * @return the parsed json object
 * @author Paul Gaudeaux
 * @date 27/09/23
 */
Json *parse_json_object(char **json_string_ptr);

/**
 * @brief parses a json array
 *
 * @sideeffects moves the pointer to string to the end of the parsed array
 * @param json_string_ptr the string of the json array
 * @return the parsed json object
 * @author Paul Gaudeaux
 * @date 27/09/23
 */
Json *parse_json_array(char **json_string_ptr);

/**
 * @brief parses a json string
 *
 * @sideeffects moves the pointer to string to the end of the parsed string
 * @param json_string_ptr the string of the json string
 * @return the parsed json object
 * @author Paul Gaudeaux
 * @date 27/09/23
 */
char *parse_json_string(char **json_string_ptr);

/**
 * @brief parses a json number
 *
 * @sideeffects moves the pointer to string to the end of the parsed number
 * @param json_string_ptr the string of the json number
 * @return the parsed json number
 * @author Paul Gaudeaux
 * @date 27/09/23
 */
int parse_json_number(char **json_string_ptr);

Json *get_json_from_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        return NULL;
    }

    // get file length
    fseek(file, 0, SEEK_END);
    long file_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // get all content of file
    char *file_content = malloc(file_length + 1);
    if (!file_content) {
        fclose(file);
        return NULL;
    }
    fread(file_content, 1, file_length, file);
    file_content[file_length] = '\0';
    fclose(file);

    // turn it into a Json struct
    Json *json = parse_json(&file_content);

    return json;
}

Json *parse_json(char **json_string) {
    // check it's not a prank
    if (!**json_string) {
        return NULL;
    }

    Json *result = NULL;

    // redirection according to object type
    switch (**json_string) {
        case '{':
            result = parse_json_object(json_string);
            break;
        case '[':
            result = parse_json_array(json_string);
            break;
        case '"':
            result = malloc(sizeof(Json));
            result->type = 's';
            result->string = parse_json_string(json_string);
            break;
        default:
            if (isdigit(**json_string)) {
                result = malloc(sizeof(Json));
                if (result) {
                    result->type = 'n';
                    result->number = parse_json_number(json_string);
                }
            } else {
                return NULL;
            }
            break;
    }

    if (result == NULL) {
        return NULL;
    }

    while (isspace((unsigned char) **json_string)) {
        (*json_string)++;
    }

    return result;
}

void free_json(Json *json) {
    // no jokes
    if (!json) {
        return;
    }

    switch (json->type) {
        case 's':
            free(json->string);
            break;
        case 'a':
            for (size_t i = 0; i < json->nb_elements; ++i) {
                free_json(&json->values[i]);
            }
            free(json->values);
            break;
        case 'o':
            for (size_t i = 0; i < json->nb_elements; ++i) {
                free(json->keys[i]);
                free_json(&json->values[i]);
            }
            free(json->keys);
            free(json->values);
            break;
        case 'n':
            break;
        default:
            break;
    }
}

Json *parse_json_object(char **json_string_ptr) {
    (*json_string_ptr)++; // '{'

    Json *obj = malloc(sizeof(Json));
    if (!obj) {
        return NULL;
    }

    obj->type = 'o';
    obj->nb_elements = 0;
    obj->keys = NULL;
    obj->values = NULL;

    while (isspace((unsigned char) **json_string_ptr)) {
        (*json_string_ptr)++;
    }

    // if it's empty, return it empty
    if (**json_string_ptr == '}') {
        (*json_string_ptr)++;
        return obj;
    }

    // get all key / value pairs
    while (1) {
        // get key
        char *key = parse_json_string(json_string_ptr);
        if (!key) {
            return NULL;
        }

        while (isspace((unsigned char) **json_string_ptr)) {
            (*json_string_ptr)++;
        }

        // if no ':' after key, bad format
        if (**json_string_ptr != ':') {
            free_json(obj);
            free(key);
            return NULL;
        }
        (*json_string_ptr)++; // ':'

        while (isspace((unsigned char) **json_string_ptr)) {
            (*json_string_ptr)++;
        }

        // Parse the value
        Json *value = parse_json(json_string_ptr);
        if (!value) {
            free_json(obj);
            free(key);
            return NULL;
        }

        obj->keys = realloc(obj->keys, (obj->nb_elements + 1) * sizeof(char *));
        obj->values = realloc(obj->values, (obj->nb_elements + 1) * sizeof(Json));

        if (!obj->keys || !obj->values) {
            free(key);
            return NULL;
        }
        obj->keys[obj->nb_elements] = key;
        obj->values[obj->nb_elements] = *value;
        obj->nb_elements++;

        while (isspace((unsigned char) **json_string_ptr)) {
            (*json_string_ptr)++;
        }

        if (**json_string_ptr == ',') {
            // new element
            (*json_string_ptr)++;
            while (isspace((unsigned char) **json_string_ptr)) {
                (*json_string_ptr)++;
            }
        } else if (**json_string_ptr == '}') {
            // end
            (*json_string_ptr)++;
            break;
        } else {
            free_json(obj);
            return NULL;
        }
    }

    return obj;
}

Json *parse_json_array(char **json_string_ptr) {
    (*json_string_ptr)++; // '['

    Json *arr = malloc(sizeof(Json));
    if (!arr) {
        return NULL;
    }

    arr->type = 'a';
    arr->nb_elements = 0;
    arr->values = NULL;

    while (isspace((unsigned char) **json_string_ptr)) {
        (*json_string_ptr)++;
    }

    // if empty, return it empty
    if (**json_string_ptr == ']') {
        (*json_string_ptr)++;
        return arr;
    }

    while (1) {
        // get value of element
        Json *value = parse_json(json_string_ptr);
        if (!value) {
            for (size_t i = 0; i < arr->nb_elements; i++) {
                free_json(&arr->values[i]);
            }
            free(arr->values);
            free(arr);
            return NULL;
        }

        // add it to the array
        arr->values = realloc(arr->values, (arr->nb_elements + 1) * sizeof(Json));
        if (!arr->values) {
            return NULL;
        }
        arr->values[arr->nb_elements] = *value;
        (arr->nb_elements)++;

        while (isspace((unsigned char) **json_string_ptr)) {
            (*json_string_ptr)++;
        }

        if (**json_string_ptr == ',') {
            (*json_string_ptr)++;
            while (isspace((unsigned char) **json_string_ptr)) {
                (*json_string_ptr)++;
            }
        } else if (**json_string_ptr == ']') {
            (*json_string_ptr)++;
            break;
        } else {
            free_json(arr);
            return NULL;
        }
    }
    return arr;
}


char *parse_json_string(char **json_string_ptr) {
    (*json_string_ptr)++; // '"'
    char *start = *json_string_ptr;
    while (**json_string_ptr && **json_string_ptr != '"') {
        (*json_string_ptr)++;
    }
    size_t len = *json_string_ptr - start;
    char *str = malloc(len + 1);
    if (!str) {
        return NULL;
    }
    strncpy(str, start, len);
    str[len] = '\0';

    (*json_string_ptr)++; // '"'
    return str;
}

int parse_json_number(char **json_string_ptr) {
    char *end;
    // strtol because my IDE tells me that atoi is bad
    int num = (int)strtol(*json_string_ptr, &end, 10);
    *json_string_ptr = end;
    return num;
}

int get_index_of_key(Json * json, char * key) {
    // Check if the json pointer is NULL
    if (json == NULL || key == NULL || json->type != 'o') {
        return -1;
    }

    for (int i = 0; i < json->nb_elements; i++) {
        if (strcmp(json->keys[i], key) == 0) {
            return i;
        }
    }

    return -1;
}

Json * get_object_at_key(Json * json, char * key) {
    if (json == NULL || key == NULL || json->type != 'o') {
        return NULL;
    }

    int index = get_index_of_key(json, key);
    if (index == -1) {
        return NULL;
    }

    return &(json->values[index]);
}
