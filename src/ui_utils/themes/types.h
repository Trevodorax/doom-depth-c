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
    SDL_Color disabled;

    SDL_Color black;
    SDL_Color white;

    SDL_Color red;
    SDL_Color green;
    SDL_Color blue;

    SDL_Color cyan;
    SDL_Color magenta;
    SDL_Color yellow;

    SDL_Color white80;
    SDL_Color purple;
    SDL_Color purple80;
} sdl_color_palette_t;

typedef struct {
    color_code_t background;
    color_code_t text;
    color_code_t highlight;
    color_code_t disabled;

    color_code_t black;
    color_code_t white;

    color_code_t red;
    color_code_t green;
    color_code_t blue;

    color_code_t cyan;
    color_code_t magenta;
    color_code_t yellow;
} cli_color_palette_t;

#endif //DOOM_DEPTH_C_THEME_TYPES_H