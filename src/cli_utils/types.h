#ifndef DOOM_DEPTH_C_TYPES_H
#define DOOM_DEPTH_C_TYPES_H

#include <stddef.h>

typedef enum {
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
} color_code_t;

typedef struct {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
} cli_rect_t;

typedef enum {
    FULL_BLOCK,
    LEFT_HALF_BLOCK,
    RIGHT_HALF_BLOCK,
    LOWER_HALF_BLOCK,
    UPPER_HALF_BLOCK,
    QUARTER_BLOCK_UPPER_LEFT,
    QUARTER_BLOCK_UPPER_RIGHT,
    QUARTER_BLOCK_LOWER_LEFT,
    QUARTER_BLOCK_LOWER_RIGHT,
    LIGHT_SHADE,
    MEDIUM_SHADE,
    DARK_SHADE
} special_char_t;

typedef enum {
    CURSOR_BEGINNING,
    CURSOR_BACK,
    CURSOR_FRONT,
    CURSOR_UP,
    CURSOR_DOWN
} cursor_movement_t;

typedef struct {
    char character;
    color_code_t color;
} cli_char_t;

typedef struct {
    size_t nb_rows;
    size_t nb_cols;
    cli_char_t ** matrix;
} cli_matrix_t;

#endif //DOOM_DEPTH_C_TYPES_H
