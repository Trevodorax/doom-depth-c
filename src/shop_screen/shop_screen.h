#ifndef DOOM_DEPTH_C_SHOP_SCREEN_H
#define DOOM_DEPTH_C_SHOP_SCREEN_H

#include "../game_window/game_window.h"
#include "../entities/player/player.h"

/**
 * @brief Displays the shop screen instead of whatever was showing before
 *
 * @param game_window the window to use
 * @param player the player currently playing
 * @return EXIT_FAILURE or EXIT_SUCCESS
 */
int shop_screen(game_window_t * game_window, player_t ** player);

#endif