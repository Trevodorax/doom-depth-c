#ifndef DOOM_DEPTH_C_SDL_UTILS_H
#define DOOM_DEPTH_C_SDL_UTILS_H

#include <SDL2/SDL.h>

typedef struct {
    /**
     * 's': start menu
     * 'm': map
     * 'f': fight
     * 'p': preferences menu
     */
    char current_screen;
} window_context_t;

typedef struct {
    SDL_Window * window;
    SDL_Renderer * renderer;
    window_context_t * context;
} game_window_t;

#endif //DOOM_DEPTH_C_SDL_UTILS_H
