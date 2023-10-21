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
 * @brief returns true if there is enough space to print the letters in the given space
 *
 * @param text The text we want to print
 * @param width
 * @param height
 *
 * @return True if we can print text in there
 */
bool can_fit_ascii_art_text(const char * text, size_t width, size_t height);

/**
 * @brief Prints the ascii text if possible
 *
 * @param matrix The matrix to print it on
 * @param container The rectangle to put the text in
 * @param text The printed text
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int print_text_ascii_art(cli_matrix_t * matrix, cli_rect_t container, const char * text);

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

    if(print_text_ascii_art(matrix, rect, text) == EXIT_SUCCESS) {
        return EXIT_SUCCESS;
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

bool can_fit_ascii_art_text(const char * text, size_t width, size_t height) {
    size_t total_width = 0;
    size_t max_height = 0;

    for (size_t i = 0; text[i] != '\0'; i++) {
        if(text[i] == ' ') {
            total_width += 5;
            continue;
        }
        ascii_art_t *letter_ascii_art = get_letter_ascii_art(text[i]);

        if (letter_ascii_art && letter_ascii_art->nb_versions > 0) {
            total_width += letter_ascii_art->versions[0]->nb_cols;
            if (letter_ascii_art->versions[0]->nb_rows > max_height) {
                max_height = letter_ascii_art->versions[0]->nb_rows;
            }

            // space after letter
            total_width += 1;
        }
    }

    // Compare the total width and max height to the given dimensions
    return total_width <= width && max_height <= height;
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

int print_text_ascii_art(cli_matrix_t * matrix, cli_rect_t container, const char * text) {
    if(!can_fit_ascii_art_text(text, container.width, container.height)) {
        return EXIT_FAILURE;
    }

    size_t current_x = container.x;

    for (size_t i = 0; text[i] != '\0'; i++) {
        if(text[i] == ' ') {
            current_x += 5;
            continue;
        }

        ascii_art_t *art = get_letter_ascii_art(text[i]);

        if(!art || art->nb_versions < 1) {
            return EXIT_FAILURE;
        }

        cli_matrix_t *character_matrix = art->versions[0];

        cli_rect_t dst_rect = {
                .x = current_x,
                .y = container.y,
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
