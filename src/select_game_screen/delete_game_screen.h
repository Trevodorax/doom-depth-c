#ifndef DOOM_DEPTH_C_DELETE_GAME_SCREEN_H
#define DOOM_DEPTH_C_DELETE_GAME_SCREEN_H

#include "../ui_utils/sdl_utils/sdl_utils.h"
#include "../entities/player/player.h"
#include "display/display.h"
#include "../event/event.h"
#include "../storage/database/database_queries.h"
#include "../storage/database/database.h"
#include "../utils/utils.h"
#include "../help_screen/help_screen.h"
#include "../confirm_quit_screen/confirm_quit_screen.h"

/**
 * @brief switches the window to load game screen which contains a welcome message and asks the player for their name
 *
 * It starts the start load game event loop, and prints the load game screen on the screen instead of what was previously there
 *
 * @param game_window the window to display the map on
 * @param player the player currently playing
 * @param db the database connection
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int delete_game_screen(game_window_t * game_window, sqlite3 * db, char * title);

#endif