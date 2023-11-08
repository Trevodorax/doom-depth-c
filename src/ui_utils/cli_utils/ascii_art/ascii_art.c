#include "ascii_art.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../cli_utils.h"
#include "../../../logs/log.h"

int
print_ascii_art_in_rectangle(cli_matrix_t * matrix, const char * ascii_file_path, rect_t container, alignment_t x_align,
                             alignment_t y_align) {
    if (!matrix || !ascii_file_path) {
        global_logger->error("\nprint_ascii_art_in_rectangle error: please provide all necessary arguments");
    }

    ascii_art_t * ascii_art = parse_ascii_art_file(ascii_file_path, BLACK);
    if (!ascii_art) {
        return EXIT_FAILURE;
    }

    cli_matrix_t * ascii_art_matrix = pick_ascii_art_version(ascii_art, container.w, container.h);
    if (!ascii_art_matrix) {
        return EXIT_FAILURE;
    }

    if (cli_copy_matrix(matrix, container, ascii_art_matrix, x_align, y_align) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

cli_matrix_t * pick_ascii_art_version(ascii_art_t * ascii_art, size_t max_width, size_t max_height) {
    if (ascii_art->nb_versions == 0) {
        return NULL;
    }
    size_t picked_ascii_art_index = 0;

    for (size_t i = 1; i < ascii_art->nb_versions; i++) {
        if(ascii_art->versions[i]->nb_cols > max_width || ascii_art->versions[i]->nb_rows > max_height) {
            continue;
        }

        if(ascii_art->versions[i]->nb_cols + ascii_art->versions[i]->nb_rows > ascii_art->versions[picked_ascii_art_index]->nb_cols + ascii_art->versions[picked_ascii_art_index]->nb_rows) {
            picked_ascii_art_index = i;
        }
    }

    if (ascii_art->versions[picked_ascii_art_index]->nb_cols > max_width ||
        ascii_art->versions[picked_ascii_art_index]->nb_rows > max_height) {
        return create_cli_matrix(max_height, max_width, '#', CYAN);
    }

    return ascii_art->versions[picked_ascii_art_index];
}

ascii_art_t *parse_ascii_art_file(const char *file_path, color_code_t color) {
    // get file content
    FILE *file = fopen(file_path, "r");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_content = (char *)malloc(file_size + 1);
    if (!file_content) {
        fclose(file);
        return NULL;
    }
    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0';

    fclose(file);

    // get nb_versions
    size_t nb_versions = 0;
    const char *delimiter = "\n;version;\n";
    char *pos = file_content;

    while ((pos = strstr(pos, delimiter)) != NULL) {
        nb_versions++;
        pos += strlen(delimiter);
    }

    // allocate memory
    ascii_art_t *ascii_art = (ascii_art_t *)malloc(sizeof(ascii_art_t));
    if (!ascii_art) {
        free(file_content);
        return NULL;
    }

    ascii_art->nb_versions = nb_versions;
    ascii_art->versions = (cli_matrix_t **)malloc(nb_versions * sizeof(cli_matrix_t *));
    if (!ascii_art->versions) {
        free(ascii_art);
        free(file_content);
        return NULL;
    }

    // get and store versions
    size_t i = 0;
    const char *start = file_content;
    const char *end = NULL;

    while ((end = strstr(start, delimiter)) != NULL) {
        size_t length = end - start;
        char *token = (char *)malloc(length + 1);

        strncpy(token, start, length);
        token[length] = '\0';

        ascii_art->versions[i] = ascii_art_to_matrix(token, color);

        free(token);
        start = end + strlen(delimiter);
        i++;
    }

    free(file_content);
    return ascii_art;
}

cli_matrix_t * ascii_art_to_matrix(char *raw_ascii_art, color_code_t color) {
    if (!raw_ascii_art) {
        return NULL;
    }

    // get number of rows and cols
    size_t nb_rows = 1;
    size_t nb_cols = 0;
    char *p = raw_ascii_art;
    size_t current_line_length = 0;

    while (*p) {
        if (*p == '\n') {
            nb_rows++;
            if (current_line_length > nb_cols) {
                nb_cols = current_line_length;
            }
            current_line_length = 0;
        } else {
            current_line_length++;
        }
        p++;
    }

    // allocate necessary memory
    cli_matrix_t *matrix = create_cli_matrix(nb_rows, nb_cols, ' ', WHITE);
    if (!matrix) {
        return NULL;
    }

    // set the values in the matrix
    p = raw_ascii_art;
    size_t row = 0;
    size_t col = 0;

    while (*p) {
        if (*p == '\n') {
            row++;
            col = 0;
        } else {
            matrix->matrix[row][col].character = *p;
            // TODOSOMEDAY: allow colors to be set in asciiart files
            matrix->matrix[row][col].color = color;
            col++;
        }
        p++;
    }

    return matrix;
}
