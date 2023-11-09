#include "level_up_screen.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "types.h"
#include "display/display.h"
#include "../help_screen/help_screen.h"

void level_up_upgrade_stat(player_t *player, stat_t selected_stat);

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
                case D_KEY:
                case S_KEY:
                    if (selected_stat == STAT_COUNT - 1) {
                        selected_stat = 0;
                    } else {
                        selected_stat += 1;
                    }
                    break;
                case Q_KEY:
                case Z_KEY:
                    if(selected_stat == 0) {
                        selected_stat = STAT_COUNT - 1;
                    } else {
                        selected_stat -= 1;
                    }
                    break;
                case QUIT:
                    return QUIT_GAME;
                case ENTER_KEY:
                    level_up_upgrade_stat(player, selected_stat);
                    display_level_up(game_window, player, selected_stat);
                    render_present(game_window);
                    delay(game_window->ui_type, 1000);
                    player->level += 1;
                    return MAP_SCREEN;
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

void level_up_upgrade_stat(player_t *player, stat_t selected_stat) {
    if (!player) {
        return;
    }

    switch(selected_stat) {
        case STAT_HP:
            player->hp_max += 20;
            break;
        case STAT_MANA:
            player->mana_max += 20;
            break;
        case STAT_DEFENSE:
            player->base_defense += 2;
            break;
        case STAT_ATTACK:
            player->base_attack += 3;
            break;
        case STAT_ACTION_POINTS:
            player->max_action_points += 1;
            break;
        default:
            break;
    }
}
