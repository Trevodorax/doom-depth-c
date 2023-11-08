#include "game_over.h"
#include "../utils/router.h"
#include "display/display.h"
#include "../event/event.h"
#include "../utils/utils.h"

int game_over_screen(game_window_t * game_window, player_t * player) {
    unsigned short active_option = TRY_AGAIN;
    event_t event;
    while (true){
        delay(game_window->ui_type, 50);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)){
            switch (event) {
                case QUIT:
                    return QUIT_GAME;
                case D_KEY:
                case S_KEY:
                    if(active_option == TRY_AGAIN) {
                        active_option = START_MENU;
                    }
                    break;
                case Q_KEY:
                case Z_KEY:
                    if(active_option == START_MENU) {
                        active_option = TRY_AGAIN;
                    }
                    break;
                case ENTER_KEY:
                    switch (active_option) {
                        case START_MENU:
                            return START_MENU;
                        case TRY_AGAIN:
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
        if(display_game_over(game_window, active_option) == EXIT_FAILURE) {
            return QUIT_GAME;
        }
        render_present(game_window);
    }
}