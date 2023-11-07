#include "display.h"
#include "../utils/inventory_utils.h"
#include "../../utils/utils.h"
#include "../../ui_utils/ui_utils.h"
#include "gui/gui.h"
#include "cli/cli.h"

int display_inventory(game_window_t * game_window,
                      inventory_t * inventory,
                      section_options_t active_section,
                      category_options_t active_category,
                      action_options_t active_action,
                      unsigned short active_item) {
    switch(game_window->ui_type) {
        case CLI:
            return display_inventory_cli(game_window, inventory, active_section, active_category, active_action, active_item);
        case GUI:
            return display_inventory_gui(game_window, inventory, active_section, active_category, active_action, active_item);
    }
}
