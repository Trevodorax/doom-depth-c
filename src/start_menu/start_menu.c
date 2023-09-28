#include "start_menu.h"
#include "../sdl_utils/sdl_utils.h"

int display_start_menu(game_window_t *game_window, unsigned short active_option);

int start_menu_screen(game_window_t *game_window) {
    SDL_Event e;
    int quit = 0;
    unsigned short active_option = 0;
    while (!quit){
        // TODO: link to next screens
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = 1;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DOWN && active_option == 0){
                active_option = 1;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP && active_option == 1){
                active_option = 0;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = 1;
            }
        }
        display_start_menu(game_window, active_option);
    }
    return EXIT_SUCCESS;
}

int display_start_menu(game_window_t *game_window, unsigned short active_option) {
    int window_height = 0;
    int window_width = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    // clear renderer
    SDL_RenderClear(game_window->renderer);

    // create texture for title
    SDL_Texture *title_texture = get_string_texture(
            game_window->renderer,
            "DoomDepthC",
            "../assets/PixelifySans-Bold.ttf",
            48,
            (SDL_Color) {255, 255, 255}
    );
    if (!title_texture) {
        return EXIT_FAILURE;
    }

    // get title dimensions
    int title_width = 0;
    int title_height = 0;
    SDL_QueryTexture(title_texture, NULL, NULL, &title_width, &title_height);

    // create rectangle for title
    SDL_Rect title_rect = {
            (window_width - title_width) / 2,
            (window_height - title_height) / 2 - 100,
            title_width,
            title_height
    };

    // add title to renderer
    SDL_RenderCopy(game_window->renderer, title_texture, NULL, &title_rect);

    // create texture for game loading option
    SDL_Texture *load_game_texture = get_string_texture(
            game_window->renderer,
            "Load game",
            "../assets/PixelifySans-Bold.ttf",
            24,
            (SDL_Color) {255, 255, 255}
    );
    if (!load_game_texture) {
        return EXIT_FAILURE;
    }

    // get game loading option dimensions
    int load_game_width = 0;
    int load_game_height = 0;
    SDL_QueryTexture(load_game_texture, NULL, NULL, &load_game_width, &load_game_height);

    // create rectangle for game loading option
    SDL_Rect load_game_rect = {
            (window_width - load_game_width) / 2,
            title_rect.y + 128,
            load_game_width,
            load_game_height
    };

    // add game loading option to renderer
    SDL_RenderCopy(game_window->renderer, load_game_texture, NULL, &load_game_rect);

    // create texture for new game option
    SDL_Texture *new_game_texture = get_string_texture(
            game_window->renderer,
            "New game",
            "../assets/PixelifySans-Bold.ttf",
            24,
            (SDL_Color) {255, 255, 255}
    );
    if (!new_game_texture) {
        return EXIT_FAILURE;
    }

    // get new game option dimensions
    int new_game_width = 0;
    int new_game_height = 0;
    SDL_QueryTexture(new_game_texture, NULL, NULL, &new_game_width, &new_game_height);

    // create rectangle for new game option
    SDL_Rect new_game_rect = {
            (window_width - new_game_width) / 2,
            load_game_rect.y +load_game_height + 24,
            new_game_width,
            new_game_height
    };

    // add new game option to renderer
    SDL_RenderCopy(game_window->renderer, new_game_texture, NULL, &new_game_rect);

    // create texture for cursor
    SDL_Texture *cursor_texture = get_string_texture(
            game_window->renderer,
            ">",
            "../assets/PixelifySans-Bold.ttf",
            12,
            (SDL_Color) {255, 255, 255}
    );
    if (!cursor_texture) {
        return EXIT_FAILURE;
    }

    // get game cursor dimensions
    int cursor_width = 0;
    int cursor_height = 0;
    SDL_QueryTexture(cursor_texture, NULL, NULL, &cursor_width, &cursor_height);

    // create rectangle for cursor
    SDL_Rect cursor_rect;
    if (active_option == 1) {
        cursor_rect = (SDL_Rect) {
                new_game_rect.x - cursor_width - 8,
                new_game_rect.y + cursor_height / 2,
                cursor_width,
                cursor_height
        };
    } else {
        cursor_rect = (SDL_Rect) {
                load_game_rect.x - cursor_width - 8,
                load_game_rect.y + cursor_height / 2,
                cursor_width,
                cursor_height
        };
    }

    // add cursor to renderer
    SDL_RenderCopy(game_window->renderer, cursor_texture, NULL, &cursor_rect);

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}