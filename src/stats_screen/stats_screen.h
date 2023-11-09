#ifndef DOOM_DEPTH_C_STATS_SCREEN_H
#define DOOM_DEPTH_C_STATS_SCREEN_H

#include "../game_window/game_window.h"
#include "../entities/player/player.h"

/**
 * @brief switches the window to stats screen
 *
 * It starts the level up event loop, and prints the stats on the screen instead of what was previously there
 *
 * @param game_window The window to display the screen on
 * @param player The player to print the stats for
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int stats_screen(game_window_t *game_window, player_t * player);

#endif //DOOM_DEPTH_C_STATS_SCREEN_H
