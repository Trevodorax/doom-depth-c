#include <SDL2/SDL.h>
#include <sqlite3.h>
#include "doom_depth.h"
#include "sdl_utils/sdl_utils.h"
#include "start_menu/start_menu.h"
#include "map/map.h"

int doom_depth() {
    SDL_Init(SDL_INIT_VIDEO);

    game_window_t * main_window = init_game_window();

    if(start_menu_screen(main_window) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if(map_screen(main_window, "../assets/map_1.json") == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    // printf("SQLite version: %s\n", sqlite3_libversion());

    free_game_window(main_window);
    SDL_Quit();

    return EXIT_SUCCESS;
}