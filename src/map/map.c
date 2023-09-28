#include "map.h"
#include "../storage/json/json.h"
#include "../sdl_utils/sdl_utils.h"

int display_map(game_window_t * game_window, Json *map);

int map_screen(game_window_t *game_window, char *map_file) {
    Json *map = get_json_from_file(map_file);
    if(!map) {
        fprintf(stderr, "map_screen error: could not retrieve map from file.");
        return EXIT_FAILURE;
    }

    SDL_Event e;
    int quit = 0;
    while (!quit){
        SDL_Delay(50);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = 1;
            }
            if (e.type == SDL_KEYDOWN){
                quit = 1;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = 1;
            }
        }
        display_map(game_window, map);
    }

    return EXIT_SUCCESS;
}

// TODO: add Doxygen
// TODO: add failure checks
int display_map(game_window_t * game_window, Json *map) {
    SDL_Color background_color = { 255, 255, 255, 255};
    if(set_background_color(game_window->renderer, background_color) == EXIT_FAILURE) {
        fprintf(stderr, "display_map error: could not set background color");
    }

    SDL_Rect stage_rect =  {0, 0, 50, 50};
    SDL_Color stage_color = { 150, 0, 0, 255};

    draw_fill_rect(stage_rect, stage_color, game_window->renderer);

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}
