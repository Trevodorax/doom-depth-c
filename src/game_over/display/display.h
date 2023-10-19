#ifndef DOOM_DEPTH_C_GAME_OVER_DISPLAY_H
#define DOOM_DEPTH_C_GAME_OVER_DISPLAY_H

#include "../../game_window/game_window.h"

/**
 * @brief Displays the game over screen instead of what was previously there.
 *
 * @param game_window
 * @param active_option The currently selected option
 * @return EXIT_FAILURE or EXIT_SUCCESS
 */
int display_game_over(game_window_t *game_window, unsigned short active_option);

#endif