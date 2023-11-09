#ifndef DOOM_DEPTH_C_CONFIRM_QUIT_H
#define DOOM_DEPTH_C_CONFIRM_QUIT_H

#include "../game_window/game_window.h"

/**
 * @brief switches the window to quit confirmation screen
 *
 * It starts the game over event loop, and prints the quit confirmation options on the screen instead of what was previously there
 *
 * @param game_window // the window to display the map on
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int confirm_quit_screen(game_window_t * game_window);

#endif //DOOM_DEPTH_C_CONFIRM_QUIT_H