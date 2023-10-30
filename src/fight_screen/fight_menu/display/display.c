#include "display.h"
#include "../../../ui_utils/ui_utils.h"
#include "cli/cli.h"
#include "gui/gui.h"

int display_menu(game_window_t *game_window, menu_t * menu, rect_t container, int selected_item_index, bool player_turn) {
    switch (game_window->ui_type) {
        case CLI:
            return display_menu_cli(game_window, menu, container, selected_item_index, player_turn);
        case GUI:
            return display_menu_gui(game_window, menu, container, selected_item_index, player_turn);
    }
}

