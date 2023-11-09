#include "display.h"
#include "../../ui_utils/sdl_utils/sdl_utils.h"
#include "../../utils/utils.h"
#include "../../shop_screen/display/gui/gui.h"

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

    rect_t game_over_rect = {
            window_width / 4,
            window_height / 6,
            window_width / 2,
            window_height / 6
    };

    print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(game_over_rect), "Game Over",
                        game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER);

    rect_t try_again_rect = {
            window_width / 4,
            game_over_rect.y + 1.5 * game_over_rect.h,
            window_width / 2,
            game_over_rect.h / 2
    };

    print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(try_again_rect), "Try again",
                            game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER);

    rect_t main_menu_rect = {
            window_width / 4,
            game_over_rect.y + 2.25 * game_over_rect.h,
            window_width / 2,
            game_over_rect.h / 2
    };

    print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(main_menu_rect), "Go back to main menu",
                            game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER);

    display_cursor_gui(game_window, (active_option == START_MENU) ? &main_menu_rect : &try_again_rect);

    return EXIT_SUCCESS;
}

int display_game_over_cli(game_window_t *game_window, unsigned short active_option) {
    int window_height = 0;
    int window_width = 0;
    cli_get_window_size(&window_width, &window_height);

    cli_render_clear(game_window->matrix, (cli_char_t){' ', game_window->cli_color_palette->text});

    // print title
    rect_t title_rect = {0, 0, window_width, window_height / 2};
    cli_print_text_in_rectangle(game_window->matrix, title_rect, "Game Over", game_window->cli_color_palette->text,
                                ALIGN_START, ALIGN_START,SMALL_TEXT);

    // print options
    rect_t option_1_rect = {2, window_height / 2, window_width, window_height / 4};
    rect_t option_2_rect = {2, 3 * (window_height / 4), window_width, window_height / 4};

    cli_print_text_in_rectangle(game_window->matrix, option_1_rect, "Try again", game_window->cli_color_palette->text,
                                ALIGN_START, ALIGN_START,SMALL_TEXT);
    cli_print_text_in_rectangle(game_window->matrix, option_2_rect, "Go back to main menu", game_window->cli_color_palette->text,
                                ALIGN_START, ALIGN_START, SMALL_TEXT);

    // print cursor
    rect_t cursor_rect = {0, 0, 1, 1};
    cursor_rect.y = active_option == TRY_AGAIN ? option_1_rect.y : option_2_rect.y;

    cli_print_text_in_rectangle(game_window->matrix, cursor_rect, ">", game_window->cli_color_palette->highlight,
                                ALIGN_START, ALIGN_START, SMALL_TEXT);

    return EXIT_SUCCESS;
}