#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <ctype.h>
#include "doom_depth.h"
#include "sdl_utils/sdl_utils.h"
#include "start_menu/start_menu.h"
#include "map/map.h"
#include "game_over/game_over.h"
#include "fight_screen/fight_screen.h"
#include "cli_utils/cli_utils.h"

/**
 * @brief Initializes a new game window
 *
 * This function allocates and initializes a new game window structure,
 * initializing the right parts for the requested ui_type
 *
 * @param ui_type
 * @return Pointer to the newly created game window structure
 */
game_window_t *init_game_window(ui_type_t ui_type);

int main_loop(game_window_t * main_window) {
    main_window->context->current_screen = FIGHT_SCREEN;
    while (main_window->context->current_screen != QUIT) {
        switch (main_window->context->current_screen) {
            case START_MENU :
                main_window->context->current_screen = start_menu_screen(main_window);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;
            case MAP_SCREEN :
                main_window->context->current_screen = map_screen(main_window, "../assets/map_1.json");
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;
            case FIGHT_SCREEN :
                main_window->context->current_screen = fight_screen(main_window, NULL, NULL);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;
            case GAME_OVER :
                main_window->context->current_screen = game_over_screen(main_window);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;
            case TRY_AGAIN:
            case QUIT:
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

    free_game_window(main_window, CLI);
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

game_window_t *init_game_window(ui_type_t ui_type) {
    game_window_t *game_window = malloc(sizeof(game_window_t));
    if(!game_window) {
        return NULL;
    }

    game_window->context = malloc(sizeof(window_context_t));
    if(!game_window->context) {
        return NULL;
    }

    game_window->ui_type = ui_type;

    // starting here, it is specific per ui types
    switch (ui_type) {
        case CLI: {
            int cli_width;
            int cli_height;
            cli_get_window_size(&cli_width, &cli_height);

            game_window->matrix = create_cli_matrix(cli_height, cli_width, 0, RED);
            break;
        }
        case GUI: {
            SDL_Init(SDL_INIT_VIDEO);

            game_window->window = SDL_CreateWindow(
                    "Doom depth c",
                    SDL_WINDOWPOS_CENTERED,
                    SDL_WINDOWPOS_CENTERED,
                    700,
                    500,
                    SDL_WINDOW_RESIZABLE
            );
            if(!game_window->window) {
                return NULL;
            }

            game_window->renderer = SDL_CreateRenderer(
                    game_window->window,
                    1,
                    SDL_RENDERER_ACCELERATED
            );
            if(!game_window->renderer) {
                return NULL;
            }

            if (TTF_Init()) {
                return NULL;
            }
            break;
        }
    }

    return game_window;
}


