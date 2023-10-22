#include "display.h"
#include "../../sdl_utils/sdl_utils.h"

int display_game_over_gui(game_window_t *game_window, unsigned short active_option);
int display_game_over_cli(game_window_t *game_window, unsigned short active_option);

int display_game_over(game_window_t *game_window, unsigned short active_option) {
    switch(game_window->ui_type) {
        case GUI:
            return display_game_over_gui(game_window, active_option);
        case CLI:
            return display_game_over_cli(game_window, active_option);
    }
}

int display_game_over_gui(game_window_t *game_window, unsigned short active_option) {
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

int display_game_over_cli(game_window_t *game_window, unsigned short active_option) {
    int window_height = 0;
    int window_width = 0;
    cli_get_window_size(&window_width, &window_height);

    cli_render_clear(game_window->matrix, (cli_char_t){' ', WHITE});

    // print title
    cli_rect_t title_rect = {0, 0, window_width, window_height / 2};
    cli_print_text_in_rectangle(game_window->matrix, title_rect, "Game Over", BLACK, ALIGN_START, ALIGN_START,
                                SMALL_TEXT);

    // print options
    cli_rect_t option_1_rect = {2, window_height / 2, window_width, window_height / 4};
    cli_rect_t option_2_rect = {2, 3 * (window_height / 4), window_width, window_height / 4};

    cli_print_text_in_rectangle(game_window->matrix, option_1_rect, "Try again", BLACK, ALIGN_START, ALIGN_START,
                                SMALL_TEXT);
    cli_print_text_in_rectangle(game_window->matrix, option_2_rect, "Go back to main menu", BLACK, ALIGN_START,
                                ALIGN_START, SMALL_TEXT);

    // print cursor
    cli_rect_t cursor_rect = {0, 0, 1, 1};
    cursor_rect.y = active_option == TRY_AGAIN ? option_1_rect.y : option_2_rect.y;

    cli_print_text_in_rectangle(game_window->matrix, cursor_rect, ">", RED, ALIGN_START, ALIGN_START, SMALL_TEXT);

    return EXIT_SUCCESS;
}