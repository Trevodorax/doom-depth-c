#ifndef DOOM_DEPTH_C_ASCII_ART_H
#define DOOM_DEPTH_C_ASCII_ART_H

#include <stddef.h>
#include "../cli_utils.h"

typedef struct {
    size_t nb_versions;
    cli_matrix_t ** versions;
} ascii_art_t;

/**
 * @brief Takes the biggest possible ascii that fits in the given dimensions.
 *
 * @param ascii_art The ascii art object to retrieve one version of
 * @param max_width Max width of the picked version
 * @param max_height Max height of the picked version
 * @return The right version
 */
cli_matrix_t * pick_ascii_art_version(ascii_art_t * ascii_art, size_t max_width, size_t max_height);

/**
 * @brief get an ascii_art object from a .asciiart file
 *
 * @param file_path The file path of the ascii art
 * @return The ascii_art object
 */
ascii_art_t * parse_ascii_art_file(char * file_path);

/**
 * @brief Get a cli_matrix from the string of an ascii art version
 *
 * @param raw_ascii_art The string of the ascii art
 * @return The matrix of the ascii art
 */
cli_matrix_t * ascii_art_to_matrix(char * raw_ascii_art);

#endif //DOOM_DEPTH_C_ASCII_ART_H
