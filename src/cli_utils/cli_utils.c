#include "cli_utils.h"
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>

void cli_get_window_size(int * width, int * height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    *width = w.ws_col;
    *height = w.ws_row;
}

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

int cli_render_present(cli_matrix_t * current_matrix) {
    // kept in memory between function calls
    static cli_matrix_t * previous_matrix;

    int window_width;
    int window_height;
    cli_get_window_size(&window_width, &window_height);

    if (!previous_matrix) {
        previous_matrix = create_cli_matrix(0, 0, ' ', BLACK);
    }

    // make some space in terminal if more is needed
    for (int i = 0; i < (int)current_matrix->nb_rows - (int)previous_matrix->nb_rows; i++) { // casting to int to avoid overflow when negative (just lost a LOT of time because of that)
        printf("\n");
    }

    // TODO: if the new matrix has less rows than the previous one, delete the excess rows
    // TODO: same for columns
    int excess_rows = (int)previous_matrix->nb_rows - (int)current_matrix->nb_rows;
    if(excess_rows > 0) {
        for (int i = 0; i < excess_rows; i++) {
            printf("\033[2K");
            cli_move_cursor(1, CURSOR_UP);
        }
    }

    cli_move_cursor((int) current_matrix->nb_rows, CURSOR_UP);
    cli_move_cursor(0, CURSOR_BEGINNING);

    // reprint only changed chars of the matrix
    for (size_t i = 0; i < current_matrix->nb_rows; i++) {
        cli_move_cursor(0, CURSOR_BEGINNING);
        cli_move_cursor(1, CURSOR_DOWN);
        for (size_t j = 0; j < current_matrix->nb_cols; j++) {
            if (
                i >= previous_matrix->nb_rows ||
                j >= previous_matrix->nb_cols ||
                previous_matrix->matrix[i][j].character != current_matrix->matrix[i][j].character ||
                previous_matrix->matrix[i][j].color != current_matrix->matrix[i][j].color
            ) {
                cli_print_color(current_matrix->matrix[i][j].color, "%c", current_matrix->matrix[i][j].character);
                continue;
            }

            cli_move_cursor(1, CURSOR_FRONT);
        }
        for(size_t j = current_matrix->nb_cols; j < previous_matrix->nb_cols && j < window_width; j++) {
            printf(" ");
        }
    }

    // copy current matrix for next call
    free_matrix(previous_matrix); // free the previous matrix
    previous_matrix = create_cli_matrix(current_matrix->nb_rows, current_matrix->nb_cols, ' ', BLACK);
    for (size_t i = 0; i < current_matrix->nb_rows; i++) {
        for (size_t j = 0; j < current_matrix->nb_cols; j++) {
            previous_matrix->matrix[i][j] = current_matrix->matrix[i][j];
        }
    }

    return EXIT_SUCCESS;
}

void cli_move_cursor(int nb_moves, cursor_movement_t cursor_movement) {
    if (nb_moves < 0) {
        fprintf(stderr, "\ncli_move_cursor error: could not move cursor a negative number of times.");
        return;
    }

    switch (cursor_movement) {
        case CURSOR_BEGINNING:
            printf("\033[1G");
            break;
        case CURSOR_BACK:
            printf("\033[%dD", nb_moves);
            break;
        case CURSOR_FRONT:
            printf("\033[%dC", nb_moves);
            break;
        case CURSOR_UP:
            printf("\033[%dA", nb_moves);
            break;
        case CURSOR_DOWN:
            printf("\033[%dB", nb_moves);
            break;
        default:
            fprintf(stderr, "\ncli_move_cursor error: unexpected cursor movement value");
            break;
    }

    // take movement into account
    fflush(stdout);
}

void cli_print_special_char(special_char_t printed_char, color_code_t color) {
    switch (printed_char) {
        case FULL_BLOCK:
            cli_print_color(color, "\u2588");
            break;
        case LEFT_HALF_BLOCK:
            cli_print_color(color, "\u258C");
            break;
        case RIGHT_HALF_BLOCK:
            cli_print_color(color, "\u2590");
            break;
        case LOWER_HALF_BLOCK:
            cli_print_color(color, "\u2584");
            break;
        case UPPER_HALF_BLOCK:
            cli_print_color(color, "\u2580");
            break;
        case QUARTER_BLOCK_UPPER_LEFT:
            cli_print_color(color, "\u2598");
            break;
        case QUARTER_BLOCK_UPPER_RIGHT:
            cli_print_color(color, "\u259D");
            break;
        case QUARTER_BLOCK_LOWER_LEFT:
            cli_print_color(color, "\u2596");
            break;
        case QUARTER_BLOCK_LOWER_RIGHT:
            cli_print_color(color, "\u2597");
            break;
        case LIGHT_SHADE:
            cli_print_color(color, "\u2591");
            break;
        case MEDIUM_SHADE:
            cli_print_color(color, "\u2592");
            break;
        case DARK_SHADE:
            cli_print_color(color, "\u2593");
            break;
        default:
            fprintf(stderr, "\ncli_print_special_char error: unexpected special character value");
            break;
    }
}

cli_matrix_t* create_cli_matrix(size_t nb_rows, size_t nb_cols, char default_char, color_code_t default_color) {
    cli_matrix_t *matrix = malloc(sizeof(cli_matrix_t));
    if(matrix == NULL) {
        fprintf(stderr, "\ncreate_cli_matrix error: failed memory allocation");
        return NULL;
    }

    matrix->nb_rows = nb_rows;
    matrix->nb_cols = nb_cols;

    matrix->matrix = malloc(nb_rows * sizeof(colored_char_t *));
    if(matrix->matrix == NULL) {
        fprintf(stderr, "\ncreate_cli_matrix error: failed memory allocation");
        free(matrix);
        return NULL;
    }

    // rows
    for(size_t i = 0; i < nb_rows; ++i) {
        matrix->matrix[i] = malloc(nb_cols * sizeof(colored_char_t));
        if(matrix->matrix[i] == NULL) {
            fprintf(stderr, "\ncreate_cli_matrix error: failed memory allocation");
            for(size_t j = 0; j < i; j++)
                free(matrix->matrix[j]);  // Free previously allocated rows
            free(matrix->matrix);
            free(matrix);
            return NULL;
        }

        // columns
        for(size_t j = 0; j < nb_cols; ++j) {
            matrix->matrix[i][j] = (colored_char_t){default_char, default_color};
        }
    }

    return matrix;
}

void free_matrix(cli_matrix_t * matrix) {
    if(matrix != NULL) {
        for(size_t i = 0; i < matrix->nb_rows; ++i) {
            free(matrix->matrix[i]);
        }
        free(matrix->matrix);
        free(matrix);
    }
}

int resize_cli_matrix(cli_matrix_t *matrix, size_t new_nb_rows, size_t new_nb_cols, char default_char, color_code_t default_color) {
    if (!matrix) {
        return EXIT_FAILURE;
    }

    // removed rows
    for (size_t i = new_nb_rows; i < matrix->nb_rows; ++i) {
        free(matrix->matrix[i]);
    }

    // resize rows
    matrix->matrix = realloc(matrix->matrix, new_nb_rows * sizeof(colored_char_t *));
    if (!matrix->matrix) {
        fprintf(stderr, "\nresize_cli_matrix error: failed memory allocation");
        return EXIT_FAILURE;
    }

    // added rows
    for (size_t i = matrix->nb_rows; i < new_nb_rows; ++i) {
        matrix->matrix[i] = malloc(new_nb_cols * sizeof(colored_char_t));
        if (!matrix->matrix[i]) {
            fprintf(stderr, "\nresize_cli_matrix error: failed memory allocation");
            return EXIT_FAILURE;
        }
        // new cells
        for (size_t j = 0; j < new_nb_cols; ++j) {
            matrix->matrix[i][j] = (colored_char_t){default_char, default_color};
        }
    }

    // existing rows
    for (size_t i = 0; i < matrix->nb_rows && i < new_nb_rows; ++i) {
        matrix->matrix[i] = realloc(matrix->matrix[i], new_nb_cols * sizeof(colored_char_t));
        if (!matrix->matrix[i]) {
            fprintf(stderr, "\nresize_cli_matrix error: failed memory allocation");
            return EXIT_FAILURE;
        }
        // new cells
        for (size_t j = matrix->nb_cols; j < new_nb_cols; ++j) {
            matrix->matrix[i][j] = (colored_char_t){default_char, default_color};
        }
    }

    // update dimensions
    matrix->nb_rows = new_nb_rows;
    matrix->nb_cols = new_nb_cols;

    return EXIT_SUCCESS;
}
