#ifndef DOOM_DEPTH_C_CLI_H
#define DOOM_DEPTH_C_CLI_H

#include "../../../ui_utils/cli_utils/types.h"
#include "../../../entities/inventory/inventory.h"
#include "../../../utils/items_management/types.h"

int display_inventory_cli(cli_matrix_t * cli_matrix,
                          inventory_t * inventory,
                          section_options_t active_section,
                          category_options_t active_category,
                          action_options_t active_action,
                          unsigned short active_item);

#endif //DOOM_DEPTH_C_CLI_H
