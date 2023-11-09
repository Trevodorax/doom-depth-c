#ifndef DOOM_DEPTH_C_LEVEL_UP_SCREEN_H
#define DOOM_DEPTH_C_LEVEL_UP_SCREEN_H

#include "../game_window/game_window.h"
#include "../entities/player/player.h"

/**
 * @brief switches the window to help
 *
 * It starts the help event loop, and prints the help on the screen instead of what was previously there
 *
 * @param game_window The window to display the screen on
 * @param player The player levelling up
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int help_screen(game_window_t *game_window);

#endif //DOOM_DEPTH_C_LEVEL_UP_SCREEN_H
