#include "game_over.h"
#include "../utils/router.h"
#include "display/display.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "../map_screen/map/map.h"
#include "../help_screen/help_screen.h"

int game_over_screen(game_window_t * game_window, player_t *player, map_t * map) {
    unsigned short active_option = TRY_AGAIN;
    event_t event;
    while (true){
        delay(game_window->ui_type, 50);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)){
            switch (event) {
                case h_KEY:
                    help_screen(game_window);
                    break;
                case Q_KEY:
                case QUIT:
                    return QUIT_GAME;
                case d_KEY:
                case s_KEY:
                    if (active_option == TRY_AGAIN) {
                        active_option = START_MENU;
                    }
                    break;
                case q_KEY:
                case z_KEY:
                    if (active_option == START_MENU) {
                        active_option = TRY_AGAIN;
                    }
                    break;
                case ENTER_KEY:
                    switch (active_option) {
                        case START_MENU:
                            return START_MENU;
                        case TRY_AGAIN:
                            save_player_map(player, map);
                            player_state_checkpoint(player, false);
                            return MAP_SCREEN;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }
        if (display_game_over(game_window, active_option) == EXIT_FAILURE) {
            return QUIT_GAME;
        }
        render_present(game_window);
    }
}