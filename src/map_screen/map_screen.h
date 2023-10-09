#ifndef DOOM_DEPTH_C_MAP_SCREEN_H
#define DOOM_DEPTH_C_MAP_SCREEN_H

#include "../sdl_utils/sdl_utils.h"
#include "stage/stage.h"
#include "../game_window/game_window.h"

/**
 * @brief switches the window to map mode
 *
 * It starts the map event loop, and prints the map on the screen instead of what was previously there
 *
 * @param game_window // the window to display the map on
 * @param current_map // path to the map json file
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int map_screen(game_window_t *game_window, char *current_map);

#endif //DOOM_DEPTH_C_MAP_SCREEN_H
