#include "display.h"
#include "../../utils/utils.h"
#include "../../utils/items_management/types.h"
#include "../../ui_utils/sdl_utils/sdl_utils.h"
#include "../utils/shop_utils.h"
#include "../../inventory_screen/display/display.h"
#include "../../ui_utils/ui_utils.h"
#include "../../inventory_screen/display/gui/gui.h"
#include "../../ui_utils/cli_utils/ascii_art/ascii_art.h"
#include "../../logs/log.h"
#include "../display/cli/cli.h"
#include "../display/gui/gui.h"

int display_shop(game_window_t * game_window, player_t * player,
                 section_options_t active_section, category_options_t active_category,
                 bool active_confirmation, unsigned short active_item) {
    switch (game_window->ui_type) {
        case GUI:
            return display_shop_gui(game_window, player, active_section, active_category, active_confirmation, active_item);
        case CLI:
            return display_shop_cli(game_window, player, active_section, active_category, active_confirmation, active_item);
    }
}