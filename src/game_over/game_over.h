#ifndef DOOM_DEPTH_C_GAME_OVER_H
#define DOOM_DEPTH_C_GAME_OVER_H

#include "../ui_utils/sdl_utils/sdl_utils.h"
#include "../game_window/game_window.h"

/**
 * @brief switches the window to game over screen
 *
 * It starts the game over event loop, and prints the game over options on the screen instead of what was previously there
 *
 * @param game_window // the window to display the map on
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int game_over_screen(game_window_t *game_window);

#endif