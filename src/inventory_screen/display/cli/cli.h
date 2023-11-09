#ifndef DOOM_DEPTH_C_CLI_H
#define DOOM_DEPTH_C_CLI_H

#include "../../../entities/inventory/inventory.h"
#include "../../../utils/items_management/types.h"
#include "../../../game_window/game_window.h"

int display_inventory_cli(game_window_t * game_window,
                          inventory_t * inventory,
                          section_options_t active_section,
                          category_options_t active_category,
                          action_options_t active_action,
                          unsigned short active_item);

#endif //DOOM_DEPTH_C_CLI_H
