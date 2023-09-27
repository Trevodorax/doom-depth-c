#include <SDL2/SDL.h>
#include <sqlite3.h>
#include "doom_depth.h"
#include "sdl_utils/sdl_utils.h"
#include "map/map.h"

int doom_depth() {
    SDL_Init(SDL_INIT_VIDEO);

    // TODO: extract to an init function
    game_window_t *main_window = malloc(sizeof(game_window_t));
    main_window->window = SDL_CreateWindow("Doom depth c", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 500, SDL_WINDOW_RESIZABLE);;
    main_window->renderer = SDL_CreateRenderer(
            main_window->window,
            1,
            SDL_RENDERER_ACCELERATED
    );
    main_window->context = malloc(sizeof(window_context_t));

    map_screen(main_window, "map_1.json");

    // printf("SQLite version: %s\n", sqlite3_libversion());

    // TODO: free the window
    SDL_Quit();

    return 0;
}