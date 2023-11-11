#include "delete_game_screen.h"

int delete_game_screen(game_window_t * game_window, sqlite3 * db, char * title) {

    if (!game_window) {
        global_logger->error("Cannot display new game initialization : no game window");
        return EXIT_FAILURE;
    }

    event_t event;
    array_node_t *players = get_players_from_db(db);
    char *zErrMsg = NULL;

    unsigned short active_option = 0;

    while (true) {
        delay(game_window->ui_type, 200);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)) {
            switch (event) {
                case h_KEY:
                    help_screen(game_window);
                    break;
                case Q_KEY:
                case QUIT:
                    if (confirm_quit_screen(game_window) == QUIT_GAME) {
                        return QUIT_GAME;
                    }
                    break;
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
                        player_t *player = get_value_at_index(players, active_option);

                        char sql_query[300];
                        sprintf(sql_query, delete_player_from_db_sql, player->id);
                        int rc = execute_query(db, sql_query, &zErrMsg);
                        if (rc != SQLITE_OK) {
                            global_logger->error("SQL error: %s", zErrMsg);
                            sqlite3_free(zErrMsg);
                            return EXIT_FAILURE;
                        }

                        free(player);

                        return START_MENU;
                    }
                    break;
                default:
                    break;
            }
        }
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }

        display_load_game(game_window, players, active_option, title);
        render_present(game_window);
    }

    return EXIT_SUCCESS;

}