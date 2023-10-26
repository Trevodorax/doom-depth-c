#ifndef DOOM_DEPTH_C_SHOP_SCREEN_DISPLAY_H
#define DOOM_DEPTH_C_SHOP_SCREEN_DISPLAY_H

#include "../../game_window/game_window.h"
#include "../../entities/player/player.h"
#include "../../utils/items_management/types.h"

/**
 * @brief Displays the shop screen instead of what was previously there.
 *
 * @param game_window
 * @param player
 * @return EXIT_FAILURE or EXIT_SUCCESS
 */
int display_shop(game_window_t *game_window, player_t *player,
                 section_options_t active_section, category_options_t active_category,
                 bool active_confirmation, unsigned short active_item);

#endif