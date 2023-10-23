#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <ctype.h>
#include "doom_depth.h"
#include "sdl_utils/sdl_utils.h"
#include "start_menu/start_menu.h"
#include "map_screen/map_screen.h"
#include "game_over/game_over.h"
#include "fight_screen/fight_screen.h"
#include "cli_utils/cli_utils.h"
#include "inventory_screen/inventory_screen.h"
#include "utils/array.h"
#include "entities/entities.h"
#include "new_game_screen/new_game_screen.h"

int main_loop(game_window_t * main_window) {
    // FIXME : remove later
    sqlite3 *db = db_connection();
    init_entities(db);
    array_node_t *spells = create_struct_from_db(db, "SELECT * FROM SPELL", create_spell_from_db, sizeof (spell_t));
    player_t *player = NULL;

    main_window->context->current_screen = START_MENU;
    while (main_window->context->current_screen != QUIT_GAME) {
        switch (main_window->context->current_screen) {
            case START_MENU :
                main_window->context->current_screen = start_menu_screen(main_window);
                if (main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;
            case MAP_SCREEN :
                main_window->context->current_screen = map_screen(main_window, "../assets/maps/map_1.json");
                if (main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;

            case FIGHT_SCREEN :
                // FIXME : remove test struct
                fight_t * fight = malloc(sizeof(fight_t));
                fight->enemies_list = malloc(sizeof(char*)*3);
                fight->enemies_list[0] = malloc(sizeof(char)*4);
                strcpy(fight->enemies_list[0],"bat");
                fight->enemies_list[1] = malloc(sizeof(char)*7);
                strcpy(fight->enemies_list[1],"goblin");
                fight->enemies_list[2] = malloc(sizeof(char)*6);
                strcpy(fight->enemies_list[2],"troll");

                fight->enemies_chances_to_appear = malloc(sizeof(int)*3);
                fight->enemies_chances_to_appear[0] = 20;
                fight->enemies_chances_to_appear[1] = 40;
                fight->enemies_chances_to_appear[2] = 40;
                fight->enemies_size = 3;
                fight->min_nb_enemies = 1;
                fight->max_nb_enemies = 5;

                player_t * player = create_player("TEST_PLAYER");
                player->offensive_spell = malloc(sizeof(spell_t));
                player->offensive_spell->name = malloc(sizeof(char)*10);
                strcpy(player->offensive_spell->name,"Fire Ball");
                player->offensive_spell->cost = 10;
                player->offensive_spell->id = 0;
                player->offensive_spell->type = ATTACK;
                player->offensive_spell->amount = 15;
                player->healing_spell = malloc(sizeof(spell_t));
                player->healing_spell->name = malloc(sizeof(char)*13);
                strcpy(player->healing_spell->name,"Fire Healing");
                player->healing_spell->cost = 40;
                player->healing_spell->id = 1;
                player->healing_spell->type = HEALING;
                player->healing_spell->amount = 80;
                player->defensive_spell = malloc(sizeof(spell_t));
                player->defensive_spell->name = malloc(sizeof(char)*16);
                strcpy(player->defensive_spell->name,"Fire Protection");
                player->defensive_spell->cost = 20;
                player->defensive_spell->id = 2;
                player->defensive_spell->type = DEFENSE;
                player->defensive_spell->amount = 10;

                main_window->context->current_screen = fight_screen(main_window, player, fight);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;

            case INVENTORY_SCREEN :
                main_window->context->current_screen = inventory_screen(main_window, NULL);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;

            case NEW_GAME_SCREEN :
                main_window->context->current_screen = new_game_screen(main_window, &player);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;

            case GAME_OVER :
                main_window->context->current_screen = game_over_screen(main_window);
                if (main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;
            case TRY_AGAIN:
            case QUIT_GAME:
                break;
        }
    }

    return EXIT_SUCCESS;
}

ui_type_t get_ui_type(char *ui_argument) {
    ui_type_t default_type = GUI;

    if (ui_argument == NULL) {
        char * env_ui_type = getenv("DOOM_DEPTH_UI_TYPE");
        if (env_ui_type == NULL) {
            return default_type;
        } else {
            return get_ui_type(env_ui_type);
        }
    }

    // not case-sensitive
    for (char *p = ui_argument; *p; p++) {
        *p = (char) tolower(*p);
    }

    // handled cases (containing is enough, I want to make it safe)
    if (strstr(ui_argument, "cli") != NULL) {
        return CLI;
    }
    if (strstr(ui_argument, "gui") != NULL) {
        return GUI;
    }

    // unhandled cases
    return default_type;
}


int doom_depth_gui() {
    game_window_t * main_window = init_game_window(GUI);

    int result = main_loop(main_window);

    free_game_window(main_window, GUI);
    SDL_Quit();

    return result;
};

int doom_depth_cli() {
    game_window_t * main_window = init_game_window(CLI);

    int result = main_loop(main_window);

    free_game_window(main_window, CLI);

    return result;
}

doom_depth_main doom_depth_factory(ui_type_t ui_type) {
    switch (ui_type) {
        case CLI:
            return doom_depth_cli;
        case GUI:
            return doom_depth_gui;
    }
}
