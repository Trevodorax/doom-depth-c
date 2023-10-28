#ifndef DOOM_DEPTH_C_LOAD_GAME_SCREEN_DISPLAY_H
#define DOOM_DEPTH_C_LOAD_GAME_SCREEN_DISPLAY_H

#include "../../game_window/game_window.h"
#include "../../ui_utils/sdl_utils/sdl_utils.h"
#include "../../ui_utils/ui_utils.h"
#include "../../utils/array.h"

int display_load_game(game_window_t *game_window, array_node_t *players);

#endif