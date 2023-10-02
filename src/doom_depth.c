#include <SDL2/SDL.h>
#include <sqlite3.h>
#include "doom_depth.h"
#include "sdl_utils/sdl_utils.h"
#include "start_menu/start_menu.h"
#include "map/map.h"
#include "game_over/game_over.h"
#include "utils/router.h"
#include "fight_screen/fight_screen.h"

int doom_depth() {
    SDL_Init(SDL_INIT_VIDEO);

    game_window_t * main_window = init_game_window();

    main_window->context->current_screen = START_MENU;
    while (main_window->context->current_screen != QUIT) {
        switch (main_window->context->current_screen) {
            case START_MENU :
                main_window->context->current_screen = start_menu_screen(main_window);
                if(main_window->context->current_screen == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
                break;

            case MAP :
                main_window->context->current_screen = map_screen(main_window, "../assets/map_1.json");
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
        }
    }

    free_game_window(main_window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
