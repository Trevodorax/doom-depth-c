#include "stats_screen.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "display/display.h"
#include "../help_screen/help_screen.h"

int stats_screen(game_window_t * game_window, player_t * player) {
    event_t event;
    while (true){
        delay(game_window->ui_type, 50);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)){
            switch (event) {
                case ESCAPE_KEY:
                case Q_KEY:
                case QUIT:
                    return MAP_SCREEN;
                default:
                    break;
            }
        }
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }
        if (display_stats(game_window, player) == EXIT_FAILURE) {
            return QUIT_GAME;
        }
        render_present(game_window);
    }
}
