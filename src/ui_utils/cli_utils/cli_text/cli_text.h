#ifndef DOOM_DEPTH_C_CLI_TEXT_H
#define DOOM_DEPTH_C_CLI_TEXT_H

#include "../types.h"

typedef enum {
    ALIGN_START,
    ALIGN_END,
    ALIGN_CENTER
} alignment_t;

typedef enum {
    TINY_TEXT,
    SMALL_TEXT,
    MEDIUM_TEXT
} text_size_t;

/**
 * @brief Like printf, but prints in the specified color
 *
 * @param color The color to print the text in
 * @param format The format string
 * @param ... All the other params, like in printf
 */
void cli_print_color(color_code_t color, const char *format, ...);

/**
 * @brief prints the given text in the rectangle
 *
 * Will try to print the given text as ascii art, otherwise in one line, if it can't, it will print it on multiple lines, and if it still
 * doesn't fit, it will add an ellipse at the end of the text
 * And if there is no space for the ellipse, it will print nothing
 *
 * @param matrix The modified matrix
 * @param rect The rectangle to write the text in
 * @param text The text to be printed
 * @param text_color The color of the text
 * @param x_align Horizontal text alignment
 * @param y_align Vertical text alignment
 * @param text_size Size of the text
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int cli_print_text_in_rectangle(cli_matrix_t *matrix, rect_t rect, const char *text, color_code_t text_color,
                                alignment_t x_align, alignment_t y_align, text_size_t text_size);

/**
 * @brief Retrieves the ascii art of one letter or digit
 *
 * @param letter
 * @return
 */
ascii_art_t * get_letter_ascii_art(char character);

#endif //DOOM_DEPTH_C_CLI_TEXT_H
