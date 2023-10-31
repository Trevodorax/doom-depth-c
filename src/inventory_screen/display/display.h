#ifndef DOOM_DEPTH_C_INVENTORY_DISPLAY_H
#define DOOM_DEPTH_C_INVENTORY_DISPLAY_H

#include "../../game_window/game_window.h"
#include "../../entities/inventory/inventory.h"
#include "../inventory_screen.h"
#include "../../utils/items_management/types.h"

/**
 * @brief displays inventory depending on where the user is in the user experience, three sections are available :
 * categories, items and their details, actions
 *
 * @param game_window the window on which to display the inventory
 * @param inventory the inventory to display
 * @param active_section the section of the user experience the user is in
 * @param active_category the current category the user has chosen or has their cursor on
 * @param active_action the current action the user has chosen or has their cursor on
 * @param active_item the current action the user has chosen or has their cursor on
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_inventory(game_window_t * game_window, inventory_t * inventory, section_options_t active_section, category_options_t active_category, action_options_t active_action, unsigned short active_item);

#endif