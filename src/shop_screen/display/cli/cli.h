#ifndef DOOM_DEPTH_C_SHOP_DISPLAY_CLI_H
#define DOOM_DEPTH_C_SHOP_DISPLAY_CLI_H

#include "../../../game_window/game_window.h"
#include "../../../entities/player/player.h"
#include "../../../utils/items_management/types.h"

int display_shop_cli(game_window_t *game_window, player_t *player,
                     section_options_t active_section, category_options_t active_category,
                     bool active_confirmation, unsigned short active_item);
int display_shop_items_cli(game_window_t * game_window, rect_t * items_rects, category_options_t active_category, unsigned int active_item);
int display_cursor_cli(game_window_t * game_window, rect_t aimed_container);
int display_item_confirm_cli(game_window_t * game_window, rect_t * container, rect_t * details_rects,
                             category_options_t active_category, unsigned int active_item);

#endif //DOOM_DEPTH_C_SHOP_DISPLAY_CLI_H