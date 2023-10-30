#ifndef DOOM_DEPTH_C_MAP_DISPLAY_H
#define DOOM_DEPTH_C_MAP_DISPLAY_H

#include "../../../game_window/game_window.h"
#include "../map.h"

/**
 * @brief displays map on the window
 *
 * @param game_window The window to print the map on
 * @param map The map to print there
 * @sideeffects Displays the map on the window, removing any previously printed content
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_map(game_window_t *game_window, map_t *map);

#endif //DOOM_DEPTH_C_MAP_DISPLAY_H
