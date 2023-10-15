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

int main_loop(game_window_t * main_window) {
    // FIXME : remove later
    player_t *player = create_player("aea");
    weapon_t *weapon = malloc(sizeof(weapon_t));
    weapon->name = "TEST";
    weapon->uses = 3;
    weapon->max_uses = 10;
    weapon->rarity = 3;
    weapon->min_attack = 3;
    weapon->max_attack = 5;
    weapon->id = 1;
    weapon->cost = 10;
    weapon->attacks_per_turn = 3;
    weapon->image_path = "../assets/weapons/lightsaber.png";
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;


    weapon_t *weapon2 = malloc(sizeof(weapon_t));
    weapon2->name = "TEST2";
    weapon2->uses = 3;
    weapon2->max_uses = 10;
    weapon2->rarity = 3;
    weapon2->min_attack = 3;
    weapon2->max_attack = 5;
    weapon2->id = 1;
    weapon2->cost = 10;
    weapon2->attacks_per_turn = 3;
    weapon2->image_path = "../assets/weapons/flip_flop.png";
    push(&(player->inventory->weapons_head), weapon2, sizeof(weapon_t));
    player->inventory->nb_weapons++;


    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;
    push(&(player->inventory->weapons_head), weapon, sizeof(weapon_t));
    player->inventory->nb_weapons++;


    main_window->context->current_screen = MAP_SCREEN;
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
                main_window->context->current_screen = fight_screen(main_window, NULL, NULL);
                if (main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;

            case INVENTORY_SCREEN :
                main_window->context->current_screen = inventory_screen(main_window, player);
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
