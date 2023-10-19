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

typedef struct {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
} cli_rect_t;

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
    char character;
    color_code_t color;
} cli_char_t;

typedef struct {
    size_t nb_rows;
    size_t nb_cols;
    cli_char_t ** matrix;
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

/**
 * @brief Allocates and initializes a new cli_matrix_t object.
 *
 * @param nb_rows The number of rows in the new matrix.
 * @param nb_cols The number of columns in the new matrix.
 * @param default_char The character used to initialize each cell in the matrix.
 * @param default_color The color code used to initialize each cell in the matrix.
 * @return A pointer to the newly allocated and initialized cli_matrix_t object.
 */
cli_matrix_t * create_cli_matrix(size_t nb_rows, size_t nb_cols, char default_char, color_code_t default_color);

/**
 * @brief Deallocates the memory used by a cli_matrix_t object.
 *
 * @param matrix A pointer to the cli_matrix_t object to be deallocated.
 */
void free_matrix(cli_matrix_t *matrix);

/**
 * @brief Resizes a cli_matrix_t object to new dimensions.
 *
 * @param matrix A pointer to the cli_matrix_t object to be resized.
 * @param new_nb_rows The new number of rows for the matrix.
 * @param new_nb_cols The new number of columns for the matrix.
 * @param default_char The character used to initialize new cells (if any).
 * @param default_color The color code used to initialize new cells (if any).
 */
int resize_cli_matrix(cli_matrix_t *matrix, size_t new_nb_rows, size_t new_nb_cols, char default_char, color_code_t default_color);

/**
 * @brief Draws a fill rectangle on the matrix
 *
 * Draws the part of the rectangle that appears on the matrix, with the provided char and color
 *
 * @param matrix The modified matrix
 * @param rect The rectangle to be printed
 * @param fill The char with which to draw the rectangle
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int cli_draw_fill_rect(cli_matrix_t * matrix, cli_rect_t rect, cli_char_t fill);

/**
 * @brief Draws a fill rectangle on the matrix
 *
 * Draws the stroke of the part of the rectangle that appears on the matrix, with the provided char and color
 *
 * @param matrix The modified matrix
 * @param rect The rectangle to be printed
 * @param stroke The char with which to draw the stroke of the rectangle
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int cli_draw_stroke_rect(cli_matrix_t * matrix, cli_rect_t rect, cli_char_t stroke);

/**
 * @brief prints the given text in the rectangle
 *
 * Will try to print the given text in one line, if it can't, it will print it on multiple lines, and if it still
 * doesn't fit, it will add an ellipse at the end of the text
 * And if there is no space for the ellipse, it will print nothing
 *
 * @param matrix The modified matrix
 * @param rect The rectangle to write the text in
 * @param text The text to be printed
 * @param text_color The color of the text
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int cli_print_text_in_rectangle(cli_matrix_t * matrix, cli_rect_t rect, const char * text, color_code_t text_color);

/**
 * @brief Resizes the matrix to the size of the terminal
 *
 * @param matrix The modified matrix
 * @param default_cli_char The cli char to use if matrix gets bigger
 * @return
 */
int resize_cli_matrix_to_window(cli_matrix_t * matrix, cli_char_t default_cli_char);

/**
 * @brief Fills the cli matrix with the given char
 *
 * @param matrix The modified matrix
 * @param character The character to fill it with
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int cli_render_clear(cli_matrix_t * matrix, cli_char_t character);

/**
 * @brief Wais for the given time
 *
 * @param time The time to wait in milliseconds
 */
void cli_delay(int time);

/**
 * @brief Makes the terminal send input directly to program without printing it or needing to press enter
 *
 * @param on Enable of disable the raw mode
 */
void set_cli_raw_mode(bool on);

/**
 * @brief Poll a char in stdin if there is one
 *
 * @param value The variable where to put the polled char
 *
 * @return true if there is a char
 */
int cli_poll_char(char * value);

/**
 * @brief Copy a matrix on another one.
 *
 * @param dst_matrix The matrix to copy the other one on.
 * @param dst_rect The rectangle where to print it on the destination.
 * @param src_matrix The matrix to copy.
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int cli_copy_matrix(cli_matrix_t * dst_matrix, cli_rect_t dst_rect, cli_matrix_t * src_matrix);

#endif //DOOM_DEPTH_C_CLI_UTILS_H
