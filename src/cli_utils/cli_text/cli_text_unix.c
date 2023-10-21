#include "cli_text.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void cli_print_color(color_code_t color, const char *format, ...) {
    // get unknown number of args
    va_list args;
    va_start(args, format);

    // set color
    printf("\033[0;%dm", color);

    // print formatted string
    vprintf(format, args);

    // reset the color
    printf("\033[0m");

    va_end(args);
}

int cli_print_text_in_rectangle(cli_matrix_t * matrix, cli_rect_t rect, const char * text, color_code_t text_color) {
    if (!matrix || !matrix->matrix || !text) {
        return EXIT_FAILURE;
    }

    size_t start_row = rect.y;
    size_t end_row = (rect.y + rect.height <= matrix->nb_rows) ? rect.y + rect.height : matrix->nb_rows;
    size_t start_col = rect.x;
    size_t end_col = (rect.x + rect.width <= matrix->nb_cols) ? rect.x + rect.width : matrix->nb_cols;

    size_t max_chars_in_rect = (end_row - start_row) * (end_col - start_col);
    size_t text_len = strlen(text);

    if (max_chars_in_rect < 3 && text_len >= 3) {
        return EXIT_FAILURE; // too small to print anything
    }

    char * ready_text = strdup(text);
    if (text_len > max_chars_in_rect) {
        ready_text[max_chars_in_rect - 1] = '.';
        ready_text[max_chars_in_rect - 2] = '.';
        ready_text[max_chars_in_rect - 3] = '.';
    }

    cli_char_t fill_char;
    size_t char_index = 0;

    for (size_t i = start_row; i < end_row && char_index < text_len; i++) {
        for (size_t j = start_col; j < end_col && char_index < text_len; j++) {
            fill_char.character = ready_text[char_index++];
            fill_char.color = text_color;

            matrix->matrix[i][j] = fill_char;

            if (char_index >= text_len) {
                return EXIT_SUCCESS;
            }
        }
    }

    return EXIT_SUCCESS;
}
