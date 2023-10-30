#include "cli_utils.h"
#include <stdlib.h>

void cli_get_window_size(int * width, int * height) {}

int cli_render_present(cli_matrix_t * current_matrix) {
    return EXIT_SUCCESS;
}

void cli_move_cursor(int nb_moves, cursor_movement_t cursor_movement) {}

void cli_print_special_char(special_char_t printed_char, color_code_t color) {}

cli_matrix_t * create_cli_matrix(size_t nb_rows, size_t nb_cols, char default_char, color_code_t default_color) {
    return NULL;
}

void free_matrix(cli_matrix_t * matrix) {}

int resize_cli_matrix(cli_matrix_t *matrix, size_t new_nb_rows, size_t new_nb_cols, char default_char, color_code_t default_color) {
    return EXIT_SUCCESS;
}

void cli_delay(int time) {}

int resize_cli_matrix_to_window(cli_matrix_t * matrix, cli_char_t default_cli_char) {
    return EXIT_SUCCESS;
}

int cli_draw_stroke_rect(cli_matrix_t * matrix, rect_t rect, cli_char_t stroke) {
    return EXIT_SUCCESS;
}

int cli_draw_fill_rect(cli_matrix_t * matrix, rect_t rect, cli_char_t fill) {
    return EXIT_SUCCESS;
}

int cli_render_clear(cli_matrix_t * matrix, cli_char_t character) {
    return EXIT_SUCCESS;
}

void set_cli_raw_mode(bool on) {}

int cli_poll_char(char * value) {
    return EXIT_SUCCESS;
}

int cli_copy_matrix(cli_matrix_t * dst_matrix, rect_t dst_rect, cli_matrix_t * src_matrix, alignment_t x_align,
                    alignment_t y_align) {
    return EXIT_SUCCESS;
}
