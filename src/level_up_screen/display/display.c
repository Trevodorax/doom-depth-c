#include "display.h"
#include "cli/cli.h"
#include "gui/gui.h"

int display_level_up(game_window_t *game_window, player_t *player, stat_t selected_stat) {
    switch(game_window->ui_type) {
        case CLI:
            return display_level_up_cli(game_window, player, selected_stat);
        case GUI:
            return display_level_up_gui(game_window, player, selected_stat);
    }
}
