#include "new_game_screen.h"
#include "display/display.h"
#include "../utils/utils.h"

#define PLAYER_NAME_MAX_LEN 25

void handle_name_input(SDL_Event e, char *name);

int new_game_screen(game_window_t * game_window, player_t ** player, map_t ** map) {
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
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                    if (strlen(name)) {
                        *player = create_player(name);
                        create_player_in_db(*player);

                        *map = get_player_map(*player);
                        return MAP_SCREEN;
                    }
                }
            }
            handle_name_input(e, name);
        }
        display_new_game(game_window, name);
    }
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