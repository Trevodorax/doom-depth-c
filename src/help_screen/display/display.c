#include "display.h"
#include "cli/cli.h"
#include "gui/gui.h"

int display_help(game_window_t *game_window) {
    switch(game_window->ui_type) {
        case CLI:
            return display_help_cli(game_window);
        case GUI:
            return display_help_gui(game_window);
    }
}
