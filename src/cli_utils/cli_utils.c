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

    if (!previous_matrix) {
        previous_matrix = malloc(sizeof(cli_matrix_t));
        previous_matrix->matrix = malloc(0);
        previous_matrix->nb_rows = 0;
        previous_matrix->nb_cols = 0;
    }

    // make some space in terminal if more is needed
    for (int i = 0; i < current_matrix->nb_rows - previous_matrix->nb_rows; i++) {
        printf("\n");
    }

    // cursor to beginning of matrix
    cli_move_cursor((int) current_matrix->nb_rows, CURSOR_UP);
    cli_move_cursor(0, CURSOR_BEGINNING);

    // reprint only changed chars of the matrix
    for (int i = 0; i < current_matrix->nb_rows; i++) {
        cli_move_cursor(0, CURSOR_BEGINNING);
        cli_move_cursor(1, CURSOR_DOWN);
        for (int j = 0; j < current_matrix->nb_cols; j++) {
            if (
                previous_matrix->nb_rows <= i ||
                previous_matrix->nb_cols <= i ||
                previous_matrix->matrix[i][j] != current_matrix->matrix[i][j]
            ) {
                printf("%c", current_matrix->matrix[i][j]);
                continue;
            }

            cli_move_cursor(1, CURSOR_FRONT);
        }
    }

    // copy current matrix for next call
    previous_matrix->matrix = realloc(previous_matrix->matrix, sizeof(char *) * current_matrix->nb_rows);
    for (int i = 0; i < current_matrix->nb_rows; i++) {
        previous_matrix->matrix[i] = realloc(previous_matrix->matrix[i], current_matrix->nb_cols);
        strcpy(previous_matrix->matrix[i], current_matrix->matrix[i]);
    }

    previous_matrix->nb_rows = current_matrix->nb_rows;
    previous_matrix->nb_cols = current_matrix->nb_cols;

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
