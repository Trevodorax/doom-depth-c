#include "fight_screen.h"
#include "fight_menu/fight_menu.h"

void update_section_dimensions(SDL_Window * window, SDL_Rect * fight_zone, SDL_Rect * menu_zone);

int fight_screen(game_window_t * game_window, player_t * player, fight_t * fight) {
    SDL_Rect fight_zone;
    SDL_Rect menu_zone;

    menu_t * menu = build_nested_menu();

    SDL_Event e;
    bool quit = false;
    while (!quit){
        SDL_Delay(50);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
        update_section_dimensions(game_window->window, &fight_zone, &menu_zone);
        display_menu(game_window->renderer, menu, &menu_zone, 0);
        SDL_RenderPresent(game_window->renderer);
    }

    return EXIT_SUCCESS;
}

void update_section_dimensions(SDL_Window * window, SDL_Rect * fight_zone, SDL_Rect * menu_zone) {
    // get the dimensions of window
    int window_height = 0;
    int window_width = 0;
    SDL_GetWindowSize(window, &window_width, &window_height);

    // use them to define zones for different zones
    fight_zone->x = 0;
    fight_zone->y = 0;
    fight_zone->w = window_width;
    fight_zone->h = window_height / 2;

    menu_zone->x = 0;
    menu_zone->y = window_height / 2;
    menu_zone->w = window_width;
    menu_zone->h = window_height / 2;
}
