#include "cli_text.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../utils/utils.h"
#include "../ascii_art/ascii_art.h"

/**
 * @brief Gets the ascii arts stored in the files.
 *
 * @param lowercase_ascii_arts
 * @param uppercase_ascii_arts
 * @param digit_ascii_arts
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int get_letters_ascii_arts(ascii_art_t ***lowercase_ascii_arts, ascii_art_t ***uppercase_ascii_arts,
                           ascii_art_t ***digit_ascii_arts, color_code_t color);

/**
 * @brief Prints the ascii text if possible
 *
 * @param matrix The matrix to print it on
 * @param container The rectangle to put the text in
 * @param text The printed text
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int
print_text_ascii_art(cli_matrix_t *matrix, rect_t container, const char *text, alignment_t x_align, alignment_t y_align,
                     text_size_t text_size, color_code_t color);

/**
 * @brief Retrieves the ascii art's size
 *
 * @param width Will be set to the width
 * @param height Will be set to the height
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int get_ascii_art_text_dimensions(const char *text, size_t *width, size_t *height, text_size_t text_size);

ascii_art_t *get_special_char_ascii_art(char character, color_code_t color);

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

int cli_print_text_in_rectangle(cli_matrix_t *matrix, rect_t rect, const char *text, color_code_t text_color,
                                alignment_t x_align, alignment_t y_align, text_size_t text_size) {
    if (!matrix || !matrix->matrix || !text) {
        return EXIT_FAILURE;
    }

    if(text_size > TINY_TEXT) {
        for(int i = (int)(text_size - 1); i >= 0; i--) {
            if (print_text_ascii_art(matrix, rect, text, x_align, y_align, i, text_color) == EXIT_SUCCESS) {
                return EXIT_SUCCESS;
            }
        }
    }

    size_t start_row = rect.y;
    size_t end_row = (rect.y + rect.h <= matrix->nb_rows) ? rect.y + rect.h : matrix->nb_rows;
    size_t start_col = rect.x;
    size_t end_col = (rect.x + rect.w <= matrix->nb_cols) ? rect.x + rect.w : matrix->nb_cols;

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

    size_t total_rows_needed = (text_len + (end_col - start_col) - 1) / (end_col - start_col);
    size_t available_rows = end_row - start_row;
    switch (y_align) {
        case ALIGN_END:
            start_row = end_row - total_rows_needed;
            break;
        case ALIGN_CENTER:
            start_row = start_row + (available_rows - total_rows_needed) / 2;
            break;
        case ALIGN_START:
        default:
            break;
    }

    for (size_t i = start_row; i < end_row && char_index < text_len; i++) {
        size_t row_text_len = end_col - start_col;
        if (text_len - char_index < row_text_len) {
            row_text_len = text_len - char_index;
        }

        size_t current_start_col = start_col;

        switch (x_align) {
            case ALIGN_END:
                current_start_col = end_col - row_text_len;
                break;
            case ALIGN_CENTER:
                current_start_col = start_col + ((end_col - start_col) - row_text_len) / 2;
                break;
            case ALIGN_START:
            default:
                break;
        }

        for (size_t j = current_start_col; j < end_col && char_index < text_len; j++) {
            char checked_char = ready_text[char_index++];

            if (checked_char == '\n') {
                break;
            }

            if (i >= matrix->nb_rows) {
                return EXIT_SUCCESS;
            }

            fill_char.character = checked_char;
            fill_char.color = text_color;

            matrix->matrix[i][j] = fill_char;

            if (char_index >= text_len) {
                return EXIT_SUCCESS;
            }
        }
    }

    return EXIT_SUCCESS;
}

int get_letters_ascii_arts(ascii_art_t ***lowercase_ascii_arts, ascii_art_t ***uppercase_ascii_arts,
                           ascii_art_t ***digit_ascii_arts, color_code_t color) {
    // make sure we're not opening all files each time
    static ascii_art_t *static_lowercase_ascii_arts[COLOR_CODE_COUNT][26] = {NULL};
    static ascii_art_t *static_uppercase_ascii_arts[COLOR_CODE_COUNT][26] = {NULL};
    static ascii_art_t *static_digit_ascii_arts[COLOR_CODE_COUNT][10] = {NULL};
    if (static_lowercase_ascii_arts[color - FIRST_COLOR][0]) {
        *lowercase_ascii_arts = static_lowercase_ascii_arts[color - FIRST_COLOR];
        *uppercase_ascii_arts = static_uppercase_ascii_arts[color - FIRST_COLOR];
        *digit_ascii_arts = static_digit_ascii_arts[color - FIRST_COLOR];
        return EXIT_SUCCESS;
    }

    char filepath[128] = {'\0'};

    // get all ascii arts from files
    for (int i = 0; i < 26; i++) {
        snprintf(filepath, sizeof(filepath), "../assets/ascii_text/lowercase/%c.asciiart", 'a' + i);
        static_lowercase_ascii_arts[color - FIRST_COLOR][i] = parse_ascii_art_file(filepath, color);
        if (!static_lowercase_ascii_arts[i]) {

            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < 26; i++) {
        snprintf(filepath, sizeof(filepath), "../assets/ascii_text/uppercase/%c.asciiart", 'A' + i);
        static_uppercase_ascii_arts[color - FIRST_COLOR][i] = parse_ascii_art_file(filepath, color);
        if (!static_uppercase_ascii_arts[i]) {
            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < 10; i++) {
        snprintf(filepath, sizeof(filepath), "../assets/ascii_text/digit/%d.asciiart", i);
        static_digit_ascii_arts[color - FIRST_COLOR][i] = parse_ascii_art_file(filepath, color);
        if (!static_digit_ascii_arts[i]) {
            return EXIT_FAILURE;
        }
    }

    // return
    *lowercase_ascii_arts = static_lowercase_ascii_arts[color - FIRST_COLOR];
    *uppercase_ascii_arts = static_uppercase_ascii_arts[color - FIRST_COLOR];
    *digit_ascii_arts = static_digit_ascii_arts[color - FIRST_COLOR];

    return EXIT_SUCCESS;
}

ascii_art_t *get_letter_ascii_art(char character, color_code_t color) {
    static ascii_art_t * unknown = NULL;
    if(!unknown) {
        unknown = parse_ascii_art_file("../assets/ascii_text/special/unknown.asciiart", color);
    }

    char_type_t char_type = get_char_type(character);

    ascii_art_t ** lowercase_ascii_arts = NULL;
    ascii_art_t ** uppercase_ascii_arts = NULL;
    ascii_art_t ** digit_ascii_arts = NULL;

    if (get_letters_ascii_arts(
            &lowercase_ascii_arts,
            &uppercase_ascii_arts,
            &digit_ascii_arts, color) == EXIT_FAILURE) {
        return NULL;
    }

    switch(char_type) {
        case DIGIT:
            return digit_ascii_arts[character - '0'];
        case LOWERCASE:
            return lowercase_ascii_arts[character - 'a'];
        case UPPERCASE:
            return uppercase_ascii_arts[character - 'A'];
        case SPECIAL:
            return get_special_char_ascii_art(character, color);
        case INVALID:
            return unknown;
        default:
            return NULL;
    }
}

ascii_art_t *get_special_char_ascii_art(char character, color_code_t color) {
    // get special char ascii arts
    static ascii_art_t * unknown[COLOR_CODE_COUNT] = {NULL};
    if(!unknown[color - FIRST_COLOR]) {
        unknown[color - FIRST_COLOR] = parse_ascii_art_file("../assets/ascii_text/special/unknown.asciiart", color);
    }

    static ascii_art_t * left_bracket[COLOR_CODE_COUNT] = {NULL};
    if(!left_bracket[color - FIRST_COLOR]) {
        left_bracket[color - FIRST_COLOR] = parse_ascii_art_file("../assets/ascii_text/special/left_bracket.asciiart", color);
    }

    static ascii_art_t * right_bracket[COLOR_CODE_COUNT] = {NULL};
    if(!right_bracket[color - FIRST_COLOR]) {
        right_bracket[color - FIRST_COLOR] = parse_ascii_art_file("../assets/ascii_text/special/right_bracket.asciiart", color);
    }

    static ascii_art_t * greater_than[COLOR_CODE_COUNT] = {NULL};
    if(!greater_than[color - FIRST_COLOR]) {
        greater_than[color - FIRST_COLOR] = parse_ascii_art_file("../assets/ascii_text/special/greater_than.asciiart", color);
    }

    // return the right one
    switch(character) {
        case '(':
            return left_bracket[color - FIRST_COLOR];
        case ')':
            return right_bracket[color - FIRST_COLOR];
        case '>':
            return greater_than[color - FIRST_COLOR];
        default:
            return unknown[color - FIRST_COLOR];
    }
}

int
print_text_ascii_art(cli_matrix_t *matrix, rect_t container, const char *text, alignment_t x_align, alignment_t y_align,
                     text_size_t text_size, color_code_t color) {
    size_t ascii_art_width;
    size_t ascii_art_height;

    if(get_ascii_art_text_dimensions(text, &ascii_art_width, &ascii_art_height, text_size) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if(ascii_art_width > container.w || ascii_art_height > container.h) {
        return EXIT_FAILURE;
    }

    // process x with alignment
    size_t current_x;
    switch(x_align) {
        case ALIGN_START:
            current_x = container.x;
            break;
        case ALIGN_END:
            current_x = container.x + container.w - ascii_art_width;
            break;
        case ALIGN_CENTER:
            current_x = container.x + container.w / 2 - ascii_art_width / 2;
            break;
        default:
            current_x = container.x;
    }

    size_t current_y;
    switch(y_align) {
        case ALIGN_START:
            current_y = container.y;
            break;
        case ALIGN_END:
            current_y = container.y + container.h - ascii_art_height;
            break;
        case ALIGN_CENTER:
            current_y = container.y + container.h / 2 - ascii_art_height / 2;
            break;
        default:
            current_y = container.y;
    }

    for (size_t i = 0; text[i] != '\0'; i++) {
        if(text[i] == ' ') {
            current_x += 5;
            continue;
        }

        ascii_art_t *art = get_letter_ascii_art(text[i], color);

        if(!art || art->nb_versions < 1) {
            return EXIT_FAILURE;
        }

        cli_matrix_t *character_matrix = art->versions[text_size];

        rect_t dst_rect = {
                .x = current_x,
                .y = current_y,
                .w = character_matrix->nb_cols,
                .h = character_matrix->nb_rows
        };

        if (cli_copy_matrix(matrix, dst_rect, character_matrix, ALIGN_START, ALIGN_START) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }

        current_x += character_matrix->nb_cols + 1;
    }

    return EXIT_SUCCESS;
}

int get_ascii_art_text_dimensions(const char *text, size_t *width, size_t *height, text_size_t text_size) {
    if (!text || !width || !height) {
        return EXIT_FAILURE;
    }

    size_t total_width = 0;
    size_t max_height = 0;

    for (size_t i = 0; text[i] != '\0'; i++) {
        if(text[i] == ' ') {
            total_width += 5;
            continue;
        }
        ascii_art_t *art = get_letter_ascii_art(text[i], BLACK);

        if(!art || !art->versions[text_size]) {
            return EXIT_FAILURE;
        }

        cli_matrix_t *character_matrix = art->versions[text_size];

        total_width += character_matrix->nb_cols;
        if (character_matrix->nb_rows > max_height) {
            max_height = character_matrix->nb_rows;
        }

        // add width for the space after each letter, except for the last one
        if (text[i + 1] != '\0') {
            total_width += 1;
        }
    }

    *width = total_width;
    *height = max_height;

    return EXIT_SUCCESS;
}
