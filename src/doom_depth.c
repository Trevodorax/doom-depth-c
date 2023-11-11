#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <ctype.h>
#include "doom_depth.h"
#include "ui_utils/sdl_utils/sdl_utils.h"
#include "start_menu/start_menu.h"
#include "map_screen/map_screen.h"
#include "game_over/game_over.h"
#include "fight_screen/fight_screen.h"
#include "ui_utils/cli_utils//cli_utils.h"
#include "inventory_screen/inventory_screen.h"
#include "utils/array.h"
#include "entities/entities.h"
#include "new_game_screen/new_game_screen.h"
#include "shop_screen/shop_screen.h"
#include "load_game_screen/load_game_screen.h"
#include "level_up_screen/level_up_screen.h"

int main_loop(game_window_t * main_window) {
    sqlite3 *db = db_connection();
    init_entities(db);

    map_t * map = NULL;
    player_t * player = NULL;

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
                main_window->context->current_screen = map_screen(main_window, &map, player);
                if (main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;

            case FIGHT_SCREEN : {
                // never happens
                main_window->context->current_screen = fight_screen(main_window, player, NULL, NULL);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;
            }

            case SHOP_SCREEN : {
                main_window->context->current_screen = shop_screen(main_window, player);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;
            }

            case NEW_GAME_SCREEN :
                main_window->context->current_screen = new_game_screen(main_window, &player, &map);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;

            case LOAD_GAME_SCREEN :
                main_window->context->current_screen = load_game_screen(main_window, &player, &map, db);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;

            case GAME_OVER :
                main_window->context->current_screen = game_over_screen(main_window, player, map);
                if (main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;

            case LEVEL_UP_SCREEN:
                main_window->context->current_screen = level_up_screen(main_window, player);
                if (main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;
            case INVENTORY_SCREEN:
            case TRY_AGAIN:
            case QUIT_GAME:
                break;
        }
    }

    // save map
    save_player_map(player, map);
    free_map(map);

    // save player
    if (save_player(db, player) != SQLITE_OK) {
        return EXIT_FAILURE;
    }

    free_player(player);

    return EXIT_SUCCESS;
}

ui_type_t get_ui_type(char * ui_argument) {
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

color_scheme_t get_color_scheme(char *color_scheme_argument) {
    color_scheme_t default_color_scheme = DARK;

    if (color_scheme_argument == NULL) {
        char * env_color_scheme = getenv("DOOM_DEPTH_COLOR_SCHEME");
        if (env_color_scheme == NULL) {
            return default_color_scheme;
        } else {
            return get_color_scheme(env_color_scheme);
        }
    }

    // not case-sensitive
    for (char *p = color_scheme_argument; *p; p++) {
        *p = (char) tolower(*p);
    }

    // handled cases (containing is enough, I want to make it safe)
    if (strstr(color_scheme_argument, "light") != NULL) {
        return LIGHT;
    }
    if (strstr(color_scheme_argument, "dark") != NULL) {
        return DARK;
    }

    // unhandled cases
    return default_color_scheme;
}

int doom_depth_gui(color_scheme_t color_scheme) {
    init_global_logger();

    if (global_logger) {
        global_logger->info("Application started");
    }

    game_window_t * main_window = init_game_window(GUI, color_scheme);

    int result = main_loop(main_window);

    free_game_window(main_window, GUI);

    if (global_logger) {
        logger_free();
    }

    return result;
}

int doom_depth_cli(color_scheme_t color_scheme) {
    init_global_logger();

    if (global_logger) {
        global_logger->info("Application started");
    }

    game_window_t * main_window = init_game_window(CLI, color_scheme);

    int result = main_loop(main_window);

    free_game_window(main_window, CLI);

    if (global_logger) {
        logger_free();
    }

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
