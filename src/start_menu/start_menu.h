#ifndef DOOM_DEPTH_C_START_MENU_H
#define DOOM_DEPTH_C_START_MENU_H

#include "../game_window/game_window.h"
#include <sqlite3.h>

/**
 * @brief switches the window to start menu
 *
 * It starts the start menu event loop, and prints the start menu on the screen instead of what was previously there
 *
 * @param game_window // the window to display the map on
 * @param db // the database connection
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int start_menu_screen(game_window_t *game_window, sqlite3 *db);

#endif

