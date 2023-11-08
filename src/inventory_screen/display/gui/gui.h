#include "../../../game_window/game_window.h"
#include "../../../entities/inventory/inventory.h"
#include "../../../utils/items_management/types.h"

#ifndef DOOM_DEPTH_C_GUI_H
#define DOOM_DEPTH_C_GUI_H

int display_inventory_gui(game_window_t * game_window,
                          inventory_t * inventory,
                          section_options_t active_section,
                          category_options_t active_category,
                          action_options_t active_action,
                          unsigned short active_item);

/**
 * @brief displays indicators at the right side of a container if the user can scroll up or down
 *
 * @param game_window the game window to use
 * @param container the rectangle in which the indicator will be
 * @param font_size the font size of the indicator
 * @param items_count the number of items displayed on one page
 * @param quantity the total number of items
 * @param first_item_to_print the index of the first item to be printed on the page
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_scroll_indicator_gui(game_window_t * game_window, SDL_Rect container, int font_size, int items_count, int quantity, int first_item_to_print);

#endif //DOOM_DEPTH_C_GUI_H
