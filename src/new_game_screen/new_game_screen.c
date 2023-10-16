#include "new_game_screen.h"
#include "../utils/router.h"
#include "display/display.h"
#include "../utils/utils.h"

#define PLAYER_NAME_MAX_LEN 25

void handle_name_input(SDL_Event e, char *name);

int new_game_screen(game_window_t *game_window, player_t *player) {
    if (!game_window) {
        printf("Cannot display new game initialization : no game window\n");
        return EXIT_FAILURE;
    }

    SDL_Event e;
    bool quit = false;
    char name[PLAYER_NAME_MAX_LEN + 1]; //do not forget null character
    name[0] = '\0';
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return QUIT_GAME;
            }
            handle_name_input(e, name);
        }
        display_new_game(game_window, name);
    }
    return EXIT_SUCCESS;
}

void handle_name_input(SDL_Event e, char *name) {
    if (e.type == SDL_TEXTINPUT) {
        if (strlen(name) < PLAYER_NAME_MAX_LEN && custom_char_check(*(e.text.text))) {
            if (strlen(name) == 0){
                strcpy(name, e.text.text);
            } else {
                strcat(name, e.text.text);
            }
        }
    }
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_BACKSPACE) {
            if (strlen(name)) {
                name[strlen(name) - 1] = '\0';
            }
        }
    }
}