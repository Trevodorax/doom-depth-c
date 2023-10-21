#ifndef DOOM_DEPTH_C_UTILS_H
#define DOOM_DEPTH_C_UTILS_H

#include "../game_window/game_window.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

#include "array.h"
#include "stdbool.h"

typedef enum {
    DIGIT,
    LOWERCASE,
    UPPERCASE,
    SPACE,
    INVALID
} char_type_t;

char* custom_itoa(int value, char* str, int base);

char* custom_strupr(char* str);

/**
 * @brief Checks if a char is valid for our system
 *
 * @param c the tested char
 * @return true if the char is valid
 */
bool custom_char_check(char c);

/**
 * @brief Processes the type of a given char
 *
 * @param c
 * @return The char type
 */
char_type_t get_char_type(char c);

void delay(ui_type_t ui_type, int ms);

#endif //DOOM_DEPTH_C_UTILS_H
