#include "start_menu.h"
#include "../event/event.h"
#include "display/display.h"
#include "../utils/utils.h"
#include "../help_screen/help_screen.h"

int start_menu_screen(game_window_t *game_window) {
    event_t event;
    unsigned short active_option = 0;
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
                    if(active_option == 0) {
                        active_option = 1;
                    }
                    break;
                case q_KEY:
                case z_KEY:
                    if(active_option == 1) {
                        active_option = 0;
                    }
                    break;
                case ENTER_KEY:
                    if(active_option == 1) {
                        return NEW_GAME_SCREEN;
                    } else {
                        return LOAD_GAME_SCREEN;
                    }
                default:
                    break;
            }
        }
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }
        if(display_start_menu(game_window, active_option) == EXIT_FAILURE) {
            return QUIT_GAME;
        }
        render_present(game_window);
    }
}
