#include "help_screen.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "types.h"
#include "display/display.h"
#include "../confirm_quit_screen/confirm_quit_screen.h"

int help_screen(game_window_t * game_window) {
    event_t event;
    while (true){
        delay(game_window->ui_type, 50);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)){
            switch (event) {
                case Q_KEY:
                case QUIT:
                    if (confirm_quit_screen(game_window) == QUIT_GAME) {
                        return QUIT_GAME;
                    }
                    break;
                default:
                    break;
            }
        }
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }
        if (display_help(game_window) == EXIT_FAILURE) {
            return QUIT_GAME;
        }
        render_present(game_window);
    }
}
