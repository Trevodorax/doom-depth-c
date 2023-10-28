#include "display.h"

int display_load_game(game_window_t *game_window, array_node_t *players) {

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    SDL_RenderClear(game_window->renderer);

    draw_fill_rect(
            (SDL_Rect){0, 0, window_width, window_height},
            (SDL_Color){0, 0, 0, 255},
            game_window->renderer
    );


    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}