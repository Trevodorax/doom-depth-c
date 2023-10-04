#ifndef DOOM_DEPTH_C_CLI_UTILS_H
#define DOOM_DEPTH_C_CLI_UTILS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

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

typedef enum {
    FULL_BLOCK,
    LEFT_HALF_BLOCK,
    RIGHT_HALF_BLOCK,
    LOWER_HALF_BLOCK,
    UPPER_HALF_BLOCK,
    QUARTER_BLOCK_UPPER_LEFT,
    QUARTER_BLOCK_UPPER_RIGHT,
    QUARTER_BLOCK_LOWER_LEFT,
    QUARTER_BLOCK_LOWER_RIGHT,
    LIGHT_SHADE,
    MEDIUM_SHADE,
    DARK_SHADE
} special_char_t;

typedef enum {
    CURSOR_BEGINNING,
    CURSOR_BACK,
    CURSOR_FRONT,
    CURSOR_UP,
    CURSOR_DOWN
} cursor_movement_t;

typedef struct {
    size_t nb_rows;
    size_t nb_cols;
    char ** matrix;
} cli_matrix_t;

/**
 * @brief Rewrites the changed chars on the matrix
 *
 * @param current_matrix The matrix to print
 * @return
 */
int cli_render_present(cli_matrix_t * current_matrix);

/**
 * @brief Gets the width and height of the terminal window
 *
 * @param width Pointer to the int where the width will be stored
 * @param height Pointer to the int where the width will be stored
 */
void cli_get_window_size(int * width, int * height);

/**
 * @brief Like printf, but prints in the specified color
 *
 * @param color The color to print the text in
 * @param format The format string
 * @param ... All the other params, like in printf
 */
void cli_print_color(color_code_t color, const char *format, ...);

/**
 * @brief Moves the cursor in the CLI
 *
 * @param nb_moves Number of times to move the cursor
 * @param cursor_movement The direction in which to move it
 */
void cli_move_cursor(int nb_moves, cursor_movement_t cursor_movement);

/**
 * @brief Prints the given special char in the specified color
 *
 * @param printed_char The special char to be printed
 * @param color Optional: the color to print it in
 */
void cli_print_special_char(special_char_t printed_char, color_code_t color);

#endif //DOOM_DEPTH_C_CLI_UTILS_H
