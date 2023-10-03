#ifndef DOOM_DEPTH_C_CLI_UTILS_H
#define DOOM_DEPTH_C_CLI_UTILS_H

#include <stdio.h>
#include <stdarg.h>

typedef enum {
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
} color_code_t;

/**
 * @brief Gets the width and height of the terminal window
 *
 * @param width Pointer to the int where the width will be stored
 * @param height Pointer to the int where the width will be stored
 */
void cli_get_window_size(int * width, int * height);

void cli_print(color_code_t color, const char *format, ...);

#endif //DOOM_DEPTH_C_CLI_UTILS_H
