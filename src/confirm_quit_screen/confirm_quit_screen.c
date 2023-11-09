#include "confirm_quit_screen.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "display/display.h"

int confirm_quit_screen(game_window_t * game_window) {
    bool quit = false;
    event_t event;
    while (true){
        delay(game_window->ui_type, 50);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)){
            switch (event) {
                case d_KEY:
                case s_KEY:
                case q_KEY:
                case z_KEY:
                    quit = !quit;
                    break;
                case ENTER_KEY:
                    if (quit) {
                        return QUIT_GAME;
                    }
                    return -1;
                default:
                    break;
            }
        }
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }
        if (display_confirm_quit(game_window, quit) == EXIT_FAILURE) {
            return QUIT_GAME;
        }
        render_present(game_window);
    }
}