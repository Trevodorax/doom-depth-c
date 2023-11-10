#include "display.h"
#include "../../ui_utils/sdl_utils/sdl_utils.h"

int display_start_menu_gui(game_window_t *game_window, unsigned short active_option);
int display_start_menu_cli(game_window_t *game_window, unsigned short active_option);

int display_start_menu(game_window_t *game_window, unsigned short active_option) {
    switch(game_window->ui_type) {
        case GUI:
            return display_start_menu_gui(game_window, active_option);
        case CLI:
            return display_start_menu_cli(game_window, active_option);
    }
}

int display_start_menu_gui(game_window_t *game_window, unsigned short active_option) {
    int window_height = 0;
    int window_width = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    // clear renderer
    SDL_RenderClear(game_window->renderer);

    SDL_Rect background_rect = {
            -10,
            -10,
            window_width + 20,
            window_height + 20
    };

    draw_image_in_rectangle(game_window->renderer, background_rect, "../assets/backgrounds/flames_around.png", NORTH,
                            false, ALIGN_START, ALIGN_START);

    // create texture for title
    SDL_Texture *title_texture = get_string_texture(
            game_window->renderer,
            "DoomDepthC",
            "../assets/PixelifySans-Bold.ttf",
            48,
            game_window->sdl_color_palette->text
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
    SDL_DestroyTexture(title_texture);

    // create texture for game loading option
    SDL_Texture *loaded_game_texture = get_string_texture(
            game_window->renderer,
            "Load game",
            "../assets/PixelifySans-Bold.ttf",
            24,
            game_window->sdl_color_palette->text
    );
    if (!loaded_game_texture) {
        return EXIT_FAILURE;
    }

    // get game loading option dimensions
    int loaded_game_width = 0;
    int loaded_game_height = 0;
    SDL_QueryTexture(loaded_game_texture, NULL, NULL, &loaded_game_width, &loaded_game_height);

    // create rectangle for game loading option
    SDL_Rect loaded_game_rect = {
            (window_width - loaded_game_width) / 2,
            title_rect.y + 128,
            loaded_game_width,
            loaded_game_height
    };

    // add game loading option to renderer
    SDL_RenderCopy(game_window->renderer, loaded_game_texture, NULL, &loaded_game_rect);
    SDL_DestroyTexture(loaded_game_texture);

    // create texture for new game option
    SDL_Texture *new_game_texture = get_string_texture(
            game_window->renderer,
            "New game",
            "../assets/PixelifySans-Bold.ttf",
            24,
            game_window->sdl_color_palette->text
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
            loaded_game_rect.y +loaded_game_height + 24,
            new_game_width,
            new_game_height
    };

    // add new game option to renderer
    SDL_RenderCopy(game_window->renderer, new_game_texture, NULL, &new_game_rect);
    SDL_DestroyTexture(new_game_texture);

    // create texture for cursor
    SDL_Texture *cursor_texture = get_string_texture(
            game_window->renderer,
            ">",
            "../assets/PixelifySans-Bold.ttf",
            12,
            game_window->sdl_color_palette->text
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
                loaded_game_rect.x - cursor_width - 8,
                loaded_game_rect.y + cursor_height / 2,
                cursor_width,
                cursor_height
        };
    }

    // add cursor to renderer
    SDL_RenderCopy(game_window->renderer, cursor_texture, NULL, &cursor_rect);
    SDL_DestroyTexture(cursor_texture);

    return EXIT_SUCCESS;
}

int display_start_menu_cli(game_window_t *game_window, unsigned short active_option) {
    int window_height = 0;
    int window_width = 0;
    cli_get_window_size(&window_width, &window_height);

    cli_render_clear(game_window->matrix, (cli_char_t){' ', game_window->cli_color_palette->background});

    // print title
    rect_t title_rect = {0, 0, window_width, window_height / 2};
    cli_print_text_in_rectangle(game_window->matrix, title_rect, "Doom Depth C", game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_START, MEDIUM_TEXT);

    // print options
    rect_t option_1_rect = {2, window_height / 2, window_width, window_height / 4};
    rect_t option_2_rect = {2, 3 * (window_height / 4), window_width, window_height / 4};

    cli_print_text_in_rectangle(game_window->matrix, option_1_rect, active_option == 0 ? "> Load a game" : "Load a game", game_window->cli_color_palette->text,
                                ALIGN_START, ALIGN_START, SMALL_TEXT);
    cli_print_text_in_rectangle(game_window->matrix, option_2_rect, active_option == 1 ? "> New game" : "New game", game_window->cli_color_palette->text,
                                ALIGN_START, ALIGN_START, SMALL_TEXT);

    return EXIT_SUCCESS;
}
