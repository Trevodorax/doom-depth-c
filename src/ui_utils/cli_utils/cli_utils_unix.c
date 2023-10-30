#include "cli_utils.h"
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <poll.h>
#include <string.h>
#include <wchar.h>

void cli_get_window_size(int * width, int * height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    *width = w.ws_col;
    *height = w.ws_row;
}

int cli_render_present(cli_matrix_t * current_matrix) {
    // kept in memory between function calls
    static cli_matrix_t * previous_matrix;
    static int previous_window_width = -1;
    static int previous_window_height = -1;

    int window_width;
    int window_height;
    cli_get_window_size(&window_width, &window_height);

    if (previous_window_width == -1 || previous_window_height ==  -1) {
        previous_window_width = window_width;
        previous_window_height = window_height;
    }

    bool is_full_rewrite = window_width != previous_window_width || window_height != previous_window_height;
    
    if (is_full_rewrite) {
        previous_window_width = window_width;
        previous_window_height = window_height;

        for (int i = 0; i < window_height; i++) {
            printf("\033[2K");
            cli_move_cursor(1, CURSOR_UP);
        }
    }

    if (!previous_matrix) {
        previous_matrix = create_cli_matrix(0, 0, ' ', BLACK);
    }

    // make some space in terminal if more is needed
    for (int i = 0; i < (int)current_matrix->nb_rows - (int)previous_matrix->nb_rows; i++) { // casting to int to avoid overflow when negative (just lost a LOT of time because of that)
        printf("\n");
    }

    int excess_rows = (int)previous_matrix->nb_rows - (int)current_matrix->nb_rows;
    if (excess_rows > 0) {
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
                previous_matrix->matrix[i][j].color != current_matrix->matrix[i][j].color ||
                is_full_rewrite
            ) {
                cli_print_color(current_matrix->matrix[i][j].color, "%c", current_matrix->matrix[i][j].character);
                continue;
            }

            cli_move_cursor(1, CURSOR_FRONT);
        }
        for (size_t j = current_matrix->nb_cols; j < previous_matrix->nb_cols && j < window_width; j++) {
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
    if (matrix == NULL) {
        fprintf(stderr, "\ncreate_cli_matrix error: failed memory allocation");
        return NULL;
    }

    matrix->nb_rows = nb_rows;
    matrix->nb_cols = nb_cols;

    matrix->matrix = malloc(nb_rows * sizeof(cli_char_t *));
    if (matrix->matrix == NULL) {
        fprintf(stderr, "\ncreate_cli_matrix error: failed memory allocation");
        free(matrix);
        return NULL;
    }

    // rows
    for (size_t i = 0; i < nb_rows; ++i) {
        matrix->matrix[i] = malloc(nb_cols * sizeof(cli_char_t));
        if (matrix->matrix[i] == NULL) {
            fprintf(stderr, "\ncreate_cli_matrix error: failed memory allocation");
            for (size_t j = 0; j < i; j++)
                free(matrix->matrix[j]);  // Free previously allocated rows
            free(matrix->matrix);
            free(matrix);
            return NULL;
        }

        // columns
        for (size_t j = 0; j < nb_cols; ++j) {
            matrix->matrix[i][j] = (cli_char_t){default_char, default_color};
        }
    }

    return matrix;
}

void free_matrix(cli_matrix_t * matrix) {
    if (matrix != NULL) {
        for (size_t i = 0; i < matrix->nb_rows; ++i) {
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
    matrix->matrix = realloc(matrix->matrix, new_nb_rows * sizeof(cli_char_t *));
    if (!matrix->matrix) {
        fprintf(stderr, "\nresize_cli_matrix error: failed memory allocation");
        return EXIT_FAILURE;
    }

    // added rows
    for (size_t i = matrix->nb_rows; i < new_nb_rows; ++i) {
        matrix->matrix[i] = malloc(new_nb_cols * sizeof(cli_char_t));
        if (!matrix->matrix[i]) {
            fprintf(stderr, "\nresize_cli_matrix error: failed memory allocation");
            return EXIT_FAILURE;
        }
        // new cells
        for (size_t j = 0; j < new_nb_cols; ++j) {
            matrix->matrix[i][j] = (cli_char_t){default_char, default_color};
        }
    }

    // existing rows
    for (size_t i = 0; i < matrix->nb_rows && i < new_nb_rows; ++i) {
        matrix->matrix[i] = realloc(matrix->matrix[i], new_nb_cols * sizeof(cli_char_t));
        if (!matrix->matrix[i]) {
            fprintf(stderr, "\nresize_cli_matrix error: failed memory allocation");
            return EXIT_FAILURE;
        }
        // new cells
        for (size_t j = matrix->nb_cols; j < new_nb_cols; ++j) {
            matrix->matrix[i][j] = (cli_char_t){default_char, default_color};
        }
    }

    // update dimensions
    matrix->nb_rows = new_nb_rows;
    matrix->nb_cols = new_nb_cols;

    return EXIT_SUCCESS;
}

int cli_draw_fill_rect(cli_matrix_t * matrix, rect_t rect, cli_char_t fill) {
    if (!matrix || !matrix->matrix) {
        return EXIT_FAILURE;
    }

    size_t start_row = rect.y;
    size_t end_row = (rect.y + rect.h < matrix->nb_rows) ? rect.y + rect.h : matrix->nb_rows;
    size_t start_col = rect.x;
    size_t end_col = (rect.x + rect.w < matrix->nb_cols) ? rect.x + rect.w : matrix->nb_cols;

    for (size_t i = start_row; i < end_row; i++) {
        for (size_t j = start_col; j < end_col; j++) {
            matrix->matrix[i][j] = fill;
        }
    }

    return EXIT_SUCCESS;
}

int cli_draw_stroke_rect(cli_matrix_t * matrix, rect_t rect, cli_char_t stroke) {
    if (!matrix || !matrix->matrix) {
        return EXIT_FAILURE;
    }

    size_t start_row = rect.y;
    size_t end_row = (rect.y + rect.h <= matrix->nb_rows) ? rect.y + rect.h : matrix->nb_rows;
    size_t start_col = rect.x;
    size_t end_col = (rect.x + rect.w <= matrix->nb_cols) ? rect.x + rect.w : matrix->nb_cols;

    // top border
    if (start_row < matrix->nb_rows && rect.y >= 0) {
        for (size_t j = start_col; j < end_col; j++) {
            matrix->matrix[start_row][j] = stroke;
        }
    }

    // bottom border
    if (end_row - 1 < matrix->nb_rows && end_row > 0 && rect.y + rect.h <= matrix->nb_rows) {
        for (size_t j = start_col; j < end_col; j++) {
            matrix->matrix[end_row - 1][j] = stroke;
        }
    }

    // left border
    if (start_col < matrix->nb_cols && rect.x >= 0) {
        for (size_t i = start_row + 1; i < end_row - 1; i++) {
            matrix->matrix[i][start_col] = stroke;
        }
    }

    // right border
    if (end_col - 1 < matrix->nb_cols && end_col > 0 && rect.x + rect.w <= matrix->nb_cols) {
        for (size_t i = start_row + 1; i < end_row - 1; i++) {
            matrix->matrix[i][end_col - 1] = stroke;
        }
    }

    return EXIT_SUCCESS;
}

int resize_cli_matrix_to_window(cli_matrix_t * matrix, cli_char_t default_cli_char) {
    if (!matrix) {
        return EXIT_FAILURE;
    }

    int width, height;
    cli_get_window_size(&width, &height);

    if (width < 0 || height < 0) {
        return EXIT_FAILURE;
    }

    return resize_cli_matrix(matrix, (size_t) height, (size_t) width, default_cli_char.character,default_cli_char.color);
}

int cli_render_clear(cli_matrix_t * matrix, cli_char_t character) {
    if (!matrix || !matrix->matrix) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < matrix->nb_rows; i++) {
        for (size_t j = 0; j < matrix->nb_cols; j++) {
            matrix->matrix[i][j].character = character.character;
            matrix->matrix[i][j].color = character.color;
        }
    }

    return EXIT_SUCCESS;
}

void cli_delay(int time) {
    set_cli_raw_mode(true);
    struct timespec ts;

    // milliseconds to seconds
    ts.tv_sec = time / 1000;
    // milliseconds left to nanoseconds
    ts.tv_nsec = (time % 1000) * 1000000L;

    nanosleep(&ts, NULL);
    set_cli_raw_mode(false);
}

void set_cli_raw_mode(bool on) {
    static struct termios old_tio;

    if (on) {
        struct termios new_tio;

        // store previous settings
        tcgetattr(STDIN_FILENO, &old_tio);

        // disable line buffering and echo in terminal
        new_tio = old_tio;
        new_tio.c_lflag &=(~ICANON & ~ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    } else {
        // apply previous settings
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    }
}

int cli_poll_char(char * value) {
    // args for poll
    struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO; // stdin
    fds[0].events = POLLIN; // poll mode

    int ret = poll(fds, 1, 0);
    if (ret > 0 && (fds[0].revents & POLLIN)) {
        // there is a char
        char c;
        read(STDIN_FILENO, &c, 1);
        *value = c;
    } else {
        // there is no char
        *value = 0;
    }

    return *value != 0;
}

int cli_copy_matrix(cli_matrix_t *dst_matrix, rect_t dst_rect, cli_matrix_t *src_matrix, alignment_t x_align,
                    alignment_t y_align) {
    if (!dst_matrix || !src_matrix) {
        return EXIT_FAILURE;
    }

    // get copy bounds that fit in both matrices
    size_t max_dst_row = dst_rect.y + dst_rect.h;
    size_t max_dst_col = dst_rect.x + dst_rect.w;

    if (max_dst_row > dst_matrix->nb_rows || max_dst_col > dst_matrix->nb_cols) {
        max_dst_row = (max_dst_row > dst_matrix->nb_rows) ? dst_matrix->nb_rows : max_dst_row;
        max_dst_col = (max_dst_col > dst_matrix->nb_cols) ? dst_matrix->nb_cols : max_dst_col;
    }

    int src_offset_x = 0;
    int src_offset_y = 0;

    switch (x_align) {
        case ALIGN_START:
            src_offset_x = 0;
            break;
        case ALIGN_CENTER:
            src_offset_x = (int)(- dst_rect.w / 2 + src_matrix->nb_cols / 2);
            break;
        case ALIGN_END:
            src_offset_x = (int)(src_matrix->nb_cols - dst_rect.w);
            break;
    }

    switch (y_align) {
        case ALIGN_START:
            src_offset_y = 0;
            break;
        case ALIGN_CENTER:
            src_offset_y = (int)(- dst_rect.h / 2 + src_matrix->nb_rows / 2);
            break;
        case ALIGN_END:
            src_offset_y = (int)(src_matrix->nb_rows - dst_rect.h);
            break;
    }

    for (size_t i = 0; i < dst_rect.h; i++) {
        for (size_t j = 0; j < dst_rect.w; j++) {
            size_t dst_x = dst_rect.x + j;
            size_t dst_y = dst_rect.y + i;

            size_t src_x = j + src_offset_x;
            size_t src_y = i + src_offset_y;

            if (src_x < src_matrix->nb_cols && src_y < src_matrix->nb_rows && dst_x < max_dst_col && dst_y < max_dst_row) {
                dst_matrix->matrix[dst_y][dst_x] = src_matrix->matrix[src_y][src_x];
            }
        }
    }

    return EXIT_SUCCESS;
}
