#include "game_stats_screen.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "../stats_screen/stats_screen.h"
#include "../help_screen/help_screen.h"
#include "../confirm_quit_screen/confirm_quit_screen.h"
#include "display/display.h"

int game_stats_screen(game_window_t * game_window, player_t * player) {
    event_t event;
    while (true){
        delay(game_window->ui_type, 50);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)){
            switch (event) {
                case m_KEY:
                    stats_screen(game_window, player);
                    break;
                case h_KEY:
                    help_screen(game_window);
                    break;
                case Q_KEY:
                case QUIT:
                case ESCAPE_KEY:
                    return MAP_SCREEN;
                default:
                    break;
            }
        }
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }
        if (display_game_stats(game_window, player) == EXIT_FAILURE) {
            return QUIT_GAME;
        }
        render_present(game_window);
    }
}