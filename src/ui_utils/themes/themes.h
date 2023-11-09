#ifndef DOOM_DEPTH_C_THEMES_H
#define DOOM_DEPTH_C_THEMES_H

#include "../../game_window/game_window.h"
#include "types.h"

int init_colors(game_window_t * game_window, color_scheme_t color_scheme);

void init_colors_gui(sdl_color_palette_t * color_palette, color_scheme_t color_scheme);

void init_colors_cli(cli_color_palette_t * color_palette, color_scheme_t color_scheme);

#endif //DOOM_DEPTH_C_THEMES_H