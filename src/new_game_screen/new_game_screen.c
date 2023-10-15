#include "new_game_screen.h"
#include "../utils/router.h"
#include "display/display.h"

#define PLAYER_NAME_MAX_LEN 25

int new_game_screen(game_window_t *game_window, player_t *player) {
    if (!game_window) {
        printf("Cannot display new game initialization : no game window\n");
        return EXIT_FAILURE;
    }

    SDL_Event e;
    bool quit = false;
    char name[PLAYER_NAME_MAX_LEN + 1]; //do not forget null character
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                return QUIT_GAME;
            }
            if (e.type == SDL_KEYDOWN){
            }
        }
        display_new_game(game_window, name);
    }
    return EXIT_SUCCESS;
}