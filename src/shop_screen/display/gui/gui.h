#ifndef DOOM_DEPTH_C_SHOP_DISPLAY_GUI_H
#define DOOM_DEPTH_C_SHOP_DISPLAY_GUI_H

#include "../../../game_window/game_window.h"
#include "../../../entities/player/player.h"
#include "../../../utils/items_management/types.h"

int display_shop_gui(game_window_t * game_window, player_t * player,
                     section_options_t active_section, category_options_t active_category,
                     bool active_confirmation, unsigned short active_item);
int display_go_back_gui(SDL_Renderer * renderer, SDL_Rect icon_container, SDL_Rect text_container);
int display_gold_gui(SDL_Renderer * renderer, player_t * player, SDL_Rect icon_container, SDL_Rect text_container);
int display_shop_categories_gui(SDL_Renderer * renderer, rect_t * categories_container,
                                SDL_Rect go_back_icon_container, SDL_Rect go_back_text_container,
                                category_options_t active_category);
int display_merchant_gui(SDL_Renderer * renderer, SDL_Rect merchant_container, SDL_Rect message_container,
                         const char * message);
int display_shop_items_gui(SDL_Renderer * renderer, rect_t * items_container,
                           category_options_t active_category, unsigned short active_item);
int display_item_confirm_gui(SDL_Renderer * renderer, SDL_Rect window_rect, SDL_Rect container,
                             bool active_confirmation, category_options_t active_category,
                             unsigned int active_item);

#endif //DOOM_DEPTH_C_SHOP_DISPLAY_GUI_H