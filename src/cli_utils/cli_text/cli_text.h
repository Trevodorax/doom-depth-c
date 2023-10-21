#ifndef DOOM_DEPTH_C_CLI_TEXT_H
#define DOOM_DEPTH_C_CLI_TEXT_H

#include "../types.h"


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



#endif //DOOM_DEPTH_C_CLI_TEXT_H
