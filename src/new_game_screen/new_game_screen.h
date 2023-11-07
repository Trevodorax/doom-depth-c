#ifndef DOOM_DEPTH_C_NEW_GAME_SCREEN_H
#define DOOM_DEPTH_C_NEW_GAME_SCREEN_H

#include "../ui_utils/sdl_utils/sdl_utils.h"
#include "../entities/player/player.h"
#include "../map_screen/map/map.h"

/**
 * @brief switches the window to new game screen which contains a welcome message and asks the player for their name
 *
 * It starts the start new game event loop, and prints the new game screen on the screen instead of what was previously there
 *
 * @param game_window the window to display the map on
 * @param player the player currently playing
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int new_game_screen(game_window_t *game_window, player_t **player, map_t **map);

#endif