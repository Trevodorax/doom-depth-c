#include "load_game_screen.h"
#include "display/display.h"
#include "../utils/utils.h"
#include "../event/event.h"
#include "../logs/log.h"
#include "../map_screen/map/map.h"

int load_game_screen(game_window_t * game_window, player_t ** player, map_t ** map, sqlite3 * db) {

    if (!game_window) {
        global_logger->error("Cannot display new game initialization : no game window");
        return EXIT_FAILURE;
    }

    event_t event;
    array_node_t *players = get_players_from_db(db);

    unsigned short active_option = 0;

    while (true) {
        delay(game_window->ui_type, 200);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)) {
            switch (event) {
                case Q_KEY:
                case QUIT:
                    return QUIT_GAME;
                case d_KEY:
                case s_KEY:
                    active_option = (active_option + 1) % 3;
                    break;
                case q_KEY:
                case z_KEY:
                    active_option = (active_option + 2) % 3;
                    break;
                case ENTER_KEY:
                    if (players != NULL) {
                        // retrieve player
                        *player = get_value_at_index(players, active_option);
                        if (*player == NULL) {
                            return NEW_GAME_SCREEN;
                        }

                        char sql_query[300];
                        sprintf(sql_query, create_player_from_db_sql, (*player)->id);
                        array_node_t *p = create_struct_from_db(db, sql_query, create_player_from_db, sizeof (player_t));
                        *player = (player_t *) p->value;
                        free(p);

                        // retrieve map
                        *map = get_player_map(*player);

                        return MAP_SCREEN;
                    } else {
                        return NEW_GAME_SCREEN;
                    }
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