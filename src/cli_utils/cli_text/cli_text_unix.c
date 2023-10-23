#include "cli_text.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"
#include "../ascii_art/ascii_art.h"

/**
 * @brief Gets the ascii arts stored in the files.
 *
 * @param lowercase_ascii_arts
 * @param uppercase_ascii_arts
 * @param digit_ascii_arts
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int get_letters_ascii_arts(
    ascii_art_t *** lowercase_ascii_arts,
    ascii_art_t *** uppercase_ascii_arts,
    ascii_art_t *** digit_ascii_arts
);

/**
 * @brief Prints the ascii text if possible
 *
 * @param matrix The matrix to print it on
 * @param container The rectangle to put the text in
 * @param text The printed text
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int print_text_ascii_art(cli_matrix_t *matrix, cli_rect_t container, const char *text, alignment_t x_align,
                         alignment_t y_align, text_size_t text_size);

/**
 * @brief Retrieves the ascii art's size
 *
 * @param width Will be set to the width
 * @param height Will be set to the height
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int get_ascii_art_text_dimensions(const char *text, size_t *width, size_t *height, text_size_t text_size);

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

int cli_print_text_in_rectangle(cli_matrix_t *matrix, cli_rect_t rect, const char *text, color_code_t text_color,
                                alignment_t x_align, alignment_t y_align, text_size_t text_size) {
    if (!matrix || !matrix->matrix || !text) {
        return EXIT_FAILURE;
    }

    for(int i = text_size; i >= 0; i--) {
        if (print_text_ascii_art(matrix, rect, text, x_align, y_align, i) == EXIT_SUCCESS) {
            return EXIT_SUCCESS;
        }
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
            // no change needed
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

int get_letters_ascii_arts(
        ascii_art_t *** lowercase_ascii_arts,
        ascii_art_t *** uppercase_ascii_arts,
        ascii_art_t *** digit_ascii_arts
) {
    // make sure we're not opening all files each time
    static ascii_art_t *static_lowercase_ascii_arts[26] = {NULL};
    static ascii_art_t *static_uppercase_ascii_arts[26] = {NULL};
    static ascii_art_t *static_digit_ascii_arts[10] = {NULL};
    if (static_lowercase_ascii_arts[0]) {
        *lowercase_ascii_arts = static_lowercase_ascii_arts;
        *uppercase_ascii_arts = static_uppercase_ascii_arts;
        *digit_ascii_arts = static_digit_ascii_arts;
        return EXIT_SUCCESS;
    }

    char filepath[128] = {'\0'};

    // get all ascii arts from files
    for (int i = 0; i < 26; i++) {
        snprintf(filepath, sizeof(filepath), "../assets/ascii_text/lowercase/%c.asciiart", 'a' + i);
        static_lowercase_ascii_arts[i] = parse_ascii_art_file(filepath);
        if (!static_lowercase_ascii_arts[i]) {

            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < 26; i++) {
        snprintf(filepath, sizeof(filepath), "../assets/ascii_text/uppercase/%c.asciiart", 'A' + i);
        static_uppercase_ascii_arts[i] = parse_ascii_art_file(filepath);
        if (!static_uppercase_ascii_arts[i]) {
            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < 10; i++) {
        snprintf(filepath, sizeof(filepath), "../assets/ascii_text/digit/%d.asciiart", i);
        static_digit_ascii_arts[i] = parse_ascii_art_file(filepath);
        if (!static_digit_ascii_arts[i]) {
            return EXIT_FAILURE;
        }
    }

    // return
    *lowercase_ascii_arts = static_lowercase_ascii_arts;
    *uppercase_ascii_arts = static_uppercase_ascii_arts;
    *digit_ascii_arts = static_digit_ascii_arts;

    return EXIT_SUCCESS;
}

ascii_art_t * get_letter_ascii_art(char character) {
    char_type_t char_type = get_char_type(character);

    if(char_type == INVALID) {
        return NULL;
    }

    ascii_art_t ** lowercase_ascii_arts = NULL;
    ascii_art_t ** uppercase_ascii_arts = NULL;
    ascii_art_t ** digit_ascii_arts = NULL;

    if (get_letters_ascii_arts(
            &lowercase_ascii_arts,
            &uppercase_ascii_arts,
            &digit_ascii_arts
    ) == EXIT_FAILURE) {
        return NULL;
    }

    switch(char_type) {
        case DIGIT:
            return digit_ascii_arts[character - '0'];
        case LOWERCASE:
            return lowercase_ascii_arts[character - 'a'];
        case UPPERCASE:
            return uppercase_ascii_arts[character - 'A'];
        default:
            return NULL;
    }
}

int print_text_ascii_art(cli_matrix_t *matrix, cli_rect_t container, const char *text, alignment_t x_align,
                         alignment_t y_align, text_size_t text_size) {
    size_t ascii_art_width;
    size_t ascii_art_height;

    get_ascii_art_text_dimensions(text, &ascii_art_width, &ascii_art_height, text_size);

    if(ascii_art_width > container.width || ascii_art_height > container.height) {
        return EXIT_FAILURE;
    }

    // process x with alignment
    size_t current_x;
    switch(x_align) {
        case ALIGN_START:
            current_x = container.x;
            break;
        case ALIGN_END:
            current_x = container.x + container.width - ascii_art_width;
            break;
        case ALIGN_CENTER:
            current_x = container.x + container.width / 2 - ascii_art_width / 2;
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
            current_y = container.y + container.height - ascii_art_height;
            break;
        case ALIGN_CENTER:
            current_y = container.y + container.height / 2 - ascii_art_height / 2;
            break;
        default:
            current_y = container.y;
    }

    for (size_t i = 0; text[i] != '\0'; i++) {
        if(text[i] == ' ') {
            current_x += 5;
            continue;
        }

        ascii_art_t *art = get_letter_ascii_art(text[i]);

        if(!art || art->nb_versions < 1) {
            return EXIT_FAILURE;
        }

        cli_matrix_t *character_matrix = art->versions[text_size];

        cli_rect_t dst_rect = {
                .x = current_x,
                .y = current_y,
                .width = character_matrix->nb_cols,
                .height = character_matrix->nb_rows
        };

        if (cli_copy_matrix(matrix, dst_rect, character_matrix) == EXIT_FAILURE) {
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
        ascii_art_t *art = get_letter_ascii_art(text[i]);

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
