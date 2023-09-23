#include <SDL2/SDL.h>

#include <sqlite3.h>
#include <stdio.h>

int main(int argc, const char * argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *_window;
    _window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 500, SDL_WINDOW_RESIZABLE);

    SDL_Event e;
    int quit = 0;
    while (!quit){
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
    }

    printf("SQLite version: %s\n", sqlite3_libversion());

    SDL_DestroyWindow(_window);
    SDL_Quit();

    return 0;

}