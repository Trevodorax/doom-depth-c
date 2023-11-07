#ifndef DOOM_DEPTH_C_THEME_TYPES_H
#define DOOM_DEPTH_C_THEME_TYPES_H

#include "SDL.h"
#include "../cli_utils/types.h"

typedef enum {
    LIGHT,
    DARK
} color_scheme_t;

typedef struct {
    SDL_Color background;
    SDL_Color text;
    SDL_Color highlight;
} sdl_color_palette_t;

typedef struct {
    color_code_t text;
    color_code_t highlight;
} cli_color_palette_t;

#endif //DOOM_DEPTH_C_THEME_TYPES_H