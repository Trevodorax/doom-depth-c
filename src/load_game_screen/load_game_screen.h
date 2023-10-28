#ifndef DOOM_DEPTH_C_LOAD_GAME_SCREEN_H
#define DOOM_DEPTH_C_LOAD_GAME_SCREEN_H

#include "../ui_utils/sdl_utils/sdl_utils.h"
#include "../entities/player/player.h"

/**
 * @brief switches the window to load game screen which contains a welcome message and asks the player for their name
 *
 * It starts the start load game event loop, and prints the load game screen on the screen instead of what was previously there
 *
 * @param game_window the window to display the map on
 * @param player the player currently playing
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int load_game_screen(game_window_t *game_window, player_t **player);

#endif