#include "load_game_screen.h"
#include "display/display.h"
#include "../utils/utils.h"
#include "../event/event.h"
#include "../logs/log.h"

int load_game_screen(game_window_t *game_window, player_t **player, sqlite3 *db) {

    if (!game_window) {
        global_logger->error("Cannot display new game initialization : no game window");
        return EXIT_FAILURE;
    }

    event_t event;
    bool quit = false;
    array_node_t *players = get_players_from_db(db);
    unsigned short active_option = 0;

    while (!quit) {
        delay(game_window->ui_type, 50);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)) {
            switch (event) {
                case QUIT:
                    return QUIT_GAME;
                case D_KEY:
                case S_KEY:
                    active_option = (active_option + 1) % 3;
                    break;
                case Q_KEY:
                case Z_KEY:
                    active_option = (active_option + 2) % 3;
                    break;
                case ENTER_KEY:
                    if (players != NULL) {
                        *player = get_value_at_index(players, active_option);
                        if (*player == NULL) {
                            return NEW_GAME_SCREEN;
                        }

                        char sql_query[300];
                        sprintf(sql_query, create_player_from_db_sql, (*player)->id);
                        array_node_t *p = create_struct_from_db(db, sql_query, create_player_from_db, sizeof (player_t));
                        *player = (player_t *) p->value;
                        free(p);
                        return MAP_SCREEN;
                    } else {
                        return NEW_GAME_SCREEN;
                    }
                    break;
                default:
                    break;
            }
        }

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }

        display_load_game(game_window, players, active_option);
        render_present(game_window);
    }

    return EXIT_SUCCESS;
}