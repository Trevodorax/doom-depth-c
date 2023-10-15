#ifndef DOOM_DEPTH_C_NEW_GAME_SCREEN_DISPLAY_H
#define DOOM_DEPTH_C_NEW_GAME_SCREEN_DISPLAY_H

#include "../../game_window/game_window.h"

int display_new_game(game_window_t *game_window, char *name);
int display_welcome(SDL_Renderer *renderer, SDL_Rect *container, int window_width, int window_height);
int display_name_question(SDL_Renderer *renderer, SDL_Rect *container, int width_ref, int height_ref);

#endif