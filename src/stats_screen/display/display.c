#include "display.h"
#include "cli/cli.h"
#include "gui/gui.h"

int display_stats(game_window_t *game_window, player_t *player) {
    switch(game_window->ui_type) {
        case CLI:
            return display_stats_cli(game_window, player);
        case GUI:
            return display_stats_gui(game_window, player);
    }
}
