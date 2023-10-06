#include "cli_utils.h"
#include <stdlib.h>

void cli_get_window_size(int * width, int * height) {
    return;
}

void cli_print_color(color_code_t color, const char *format, ...) {
    return;
}

int cli_render_present(cli_matrix_t * current_matrix) {
    return EXIT_SUCCESS;
}

void cli_move_cursor(int nb_moves, cursor_movement_t cursor_movement) {
    return;
}

void cli_print_special_char(special_char_t printed_char, color_code_t color) {
    return;
}

cli_matrix_t* create_cli_matrix(size_t nb_rows, size_t nb_cols, char default_char, color_code_t default_color) {
    return NULL;
}

void free_matrix(cli_matrix_t * matrix) {
    return;
}

int resize_cli_matrix(cli_matrix_t *matrix, size_t new_nb_rows, size_t new_nb_cols, char default_char, color_code_t default_color) {
    return EXIT_SUCCESS;
}
