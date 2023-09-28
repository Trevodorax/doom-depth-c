#include <SDL2/SDL.h>
#include <sqlite3.h>
#include "doom_depth.h"
#include "sdl_utils/sdl_utils.h"
#include "map/map.h"

int doom_depth() {
    SDL_Init(SDL_INIT_VIDEO);

    game_window_t * main_window = init_game_window();

    map_screen(main_window, "map_1.json");

    // printf("SQLite version: %s\n", sqlite3_libversion());

    free_game_window(main_window);
    SDL_Quit();

    return 0;
}