#include "display.h"
#include "./gui/gui.h"
#include "./cli/cli.h"

int display_fight(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone) {
    switch(game_window->ui_type) {
        case CLI:
            return display_fight_cli(game_window, fight_context, fight_zone);
        case GUI:
            return display_fight_gui(game_window, fight_context, rect_to_SDL_Rect(fight_zone));
    }
}
