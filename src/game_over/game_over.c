#include "game_over.h"
#include "../sdl_utils/sdl_utils.h"
#include "../utils/router.h"

int display_game_over(game_window_t *game_window, unsigned short active_option);

int game_over_screen(game_window_t *game_window) {
    SDL_Event e;
    bool quit = false;
    unsigned short active_option = TRY_AGAIN;
    while (!quit){
        // TODO: link to try again option
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                return QUIT;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DOWN && active_option == TRY_AGAIN){
                active_option = START_MENU;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP && active_option == START_MENU){
                active_option = TRY_AGAIN;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && active_option == START_MENU){
                return START_MENU;
            }
        }
        display_game_over(game_window, active_option);
    }
    return EXIT_SUCCESS;
}

int display_game_over(game_window_t *game_window, unsigned short active_option) {
    int window_height = 0;
    int window_width = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    // clear renderer
    SDL_RenderClear(game_window->renderer);

    // create texture for game over message
    SDL_Texture *game_over_texture = get_string_texture(
            game_window->renderer,
            "Game Over",
            "../assets/PixelifySans-Bold.ttf",
            48,
            (SDL_Color) {255, 255, 255, 255}
            );
    if (!game_over_texture) {
        return EXIT_FAILURE;
    }

    // get game over message dimensions
    int game_over_height = 0;
    int game_over_width = 0;
    SDL_QueryTexture(game_over_texture, NULL, NULL, &game_over_width, &game_over_height);

    // create rectangle for game over message
    SDL_Rect game_over_rect = {
            (window_width - game_over_width) / 2,
            (window_height - game_over_height) / 2 - 100,
            game_over_width,
            game_over_height
    };

    // add game over message to renderer
    SDL_RenderCopy(game_window->renderer, game_over_texture, NULL, &game_over_rect);
    SDL_DestroyTexture(game_over_texture);


    // create texture for try again option
    SDL_Texture *try_again_texture = get_string_texture(
            game_window->renderer,
            "Try again",
            "../assets/PixelifySans-Bold.ttf",
            24,
            (SDL_Color) {255, 255, 255, 255}
    );
    if (!try_again_texture) {
        return EXIT_FAILURE;
    }

    // get try again option dimensions
    int try_again_width = 0;
    int try_again_height = 0;
    SDL_QueryTexture(try_again_texture, NULL, NULL, &try_again_width, &try_again_height);

    // create rectangle for game loading option
    SDL_Rect try_again_rect = {
            (window_width - try_again_width) / 2,
            game_over_rect.y + 128,
            try_again_width,
            try_again_height
    };

    // add try again option to renderer
    SDL_RenderCopy(game_window->renderer, try_again_texture, NULL, &try_again_rect);
    SDL_DestroyTexture(try_again_texture);


    // create texture for going back to main menu option
    SDL_Texture *main_menu_texture = get_string_texture(
            game_window->renderer,
            "Go back to main menu",
            "../assets/PixelifySans-Bold.ttf",
            24,
            (SDL_Color) {255, 255, 255, 255}
    );
    if (!main_menu_texture) {
        return EXIT_FAILURE;
    }

    // get going back to main menu option dimensions
    int main_menu_width = 0;
    int main_menu_height = 0;
    SDL_QueryTexture(main_menu_texture, NULL, NULL, &main_menu_width, &main_menu_height);

    // create rectangle for going back to main menu option
    SDL_Rect main_menu_rect = {
            (window_width - main_menu_width) / 2,
            try_again_rect.y +try_again_height + 24,
            main_menu_width,
            main_menu_height
    };

    // add going back to main menu option to renderer
    SDL_RenderCopy(game_window->renderer, main_menu_texture, NULL, &main_menu_rect);
    SDL_DestroyTexture(main_menu_texture);

    // create texture for cursor
    SDL_Texture *cursor_texture = get_string_texture(
            game_window->renderer,
            ">",
            "../assets/PixelifySans-Bold.ttf",
            12,
            (SDL_Color) {255, 255, 255, 255}
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
    if (active_option == START_MENU) {
        cursor_rect = (SDL_Rect) {
                main_menu_rect.x - cursor_width - 8,
                main_menu_rect.y + cursor_height / 2,
                cursor_width,
                cursor_height
        };
    } else {
        cursor_rect = (SDL_Rect) {
                try_again_rect.x - cursor_width - 8,
                try_again_rect.y + cursor_height / 2,
                cursor_width,
                cursor_height
        };
    }

    // add cursor to renderer
    SDL_RenderCopy(game_window->renderer, cursor_texture, NULL, &cursor_rect);
    SDL_DestroyTexture(cursor_texture);

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}