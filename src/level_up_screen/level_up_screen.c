#include "level_up_screen.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "types.h"
#include "display/display.h"

int level_up_screen(game_window_t * game_window, player_t * player) {
    stat_t selected_stat = STAT_HP;

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
                case ENTER_KEY:
                    return MAP_SCREEN;
                    // level_up_upgrade_stat(game_window, player, selected_stat);
                default:
                    break;
            }
        }
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }
        if (display_level_up(game_window, player, selected_stat) == EXIT_FAILURE) {
            return QUIT_GAME;
        }
        render_present(game_window);
    }
}
