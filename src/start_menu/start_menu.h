#ifndef DOOM_DEPTH_C_START_MENU_H
#define DOOM_DEPTH_C_START_MENU_H

#include "../sdl_utils/sdl_utils.h"

/**
 * @brief switches the window to start menu
 *
 * It starts the start menu event loop, and prints the start menu on the screen instead of what was previously there
 *
 * @param game_window // the window to display the map on
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int start_menu_screen(game_window_t *game_window);

#endif
