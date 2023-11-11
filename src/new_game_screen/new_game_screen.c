#include "new_game_screen.h"
#include "display/display.h"
#include "../utils/utils.h"
#include "../entities/player/player.h"
#include "../event/event.h"
#include "../help_screen/help_screen.h"
#include "../confirm_quit_screen/confirm_quit_screen.h"

#define PLAYER_NAME_MAX_LEN 25

void handle_name_input(event_t event, char * name);

int new_game_screen(game_window_t * game_window, player_t ** player, map_t ** map, sqlite3 * db) {
    if (!game_window) {
        printf("Cannot display new game initialization : no game window\n");
        return EXIT_FAILURE;
    }

    event_t event;
    char name[PLAYER_NAME_MAX_LEN + 1]; //do not forget null character
    name[0] = '\0';
    while (true) {
        delay(game_window->ui_type, 50);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)) {
            switch (event) {
                case Q_KEY:
                case QUIT:
                    if (confirm_quit_screen(game_window) == QUIT_GAME) {
                        return QUIT_GAME;
                    }
                    break;
                case ENTER_KEY:
                    if (strlen(name)) {
                        *player = create_player(name);
                        int player_id = create_player_in_db(*player);
                        (*player)->id = player_id;
                        char sql_query[300];
                        sprintf(sql_query, create_player_from_db_sql, (*player)->id);
                        array_node_t *p = create_struct_from_db(db, sql_query, create_player_from_db, sizeof (player_t));
                        *player = (player_t *) p->value;

                        free(p);

                        *map = get_player_map(*player);
                        return MAP_SCREEN;
                    }
                    break;
                default:
                    break;
            }
            handle_name_input(event, name);
        }

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }

        display_new_game(game_window, name);
        render_present(game_window);
    }

    return EXIT_SUCCESS;
}

void handle_name_input(event_t event, char * name) {
    char input_char = event_to_char(event);
    if ((input_char >= 'A' && input_char <= 'Z') || (input_char >= 'a' && input_char <= 'z')) {
        if (strlen(name) < PLAYER_NAME_MAX_LEN && custom_char_check(input_char)) {
            name[strlen(name)] = input_char;
            name[strlen(name) + 1] = '\0';
        }
    }
    if (event == BACKSPACE_KEY) {
        if (strlen(name)) {
            name[strlen(name) - 1] = '\0';
        }
    }
}