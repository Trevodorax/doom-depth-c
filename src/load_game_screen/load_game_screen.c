#include "display/display.h"

int load_game_screen(game_window_t *game_window, player_t **player) {

    if (!game_window) {
        printf("Cannot display new game initialization : no game window\n");
        return EXIT_FAILURE;
    }

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return QUIT_GAME;
            }
        }
        display_load_game(game_window, NULL);
    }

    return EXIT_SUCCESS;
}