#include "display.h"
#include "../../ui_utils/sdl_utils/sdl_utils.h"
#include "../../shop_screen/display/gui/gui.h"
#include "../../shop_screen/display/cli/cli.h"

int display_confirm_quit_gui(game_window_t * game_window, bool quit);
int display_confirm_quit_cli(game_window_t * game_window, bool quit);

int display_confirm_quit(game_window_t * game_window, bool quit) {
    switch (game_window->ui_type) {
        case GUI:
            return display_confirm_quit_gui(game_window, quit);
        case CLI:
            return display_confirm_quit_cli(game_window, quit);
    }
}

int display_confirm_quit_gui(game_window_t * game_window, bool quit) {
    int window_height = 0;
    int window_width = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    // clear renderer
    SDL_RenderClear(game_window->renderer);

    draw_fill_rect((SDL_Rect) {0, 0, window_width, window_height}, game_window->sdl_color_palette->background, game_window->renderer);

    rect_t question_rect = {
            window_width / 4,
            window_height / 6,
            window_width / 2,
            window_height / 6
    };

    print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(question_rect), "Bye quitter",
                            game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER);

    rect_t yes_rect = {
            window_width / 4,
            question_rect.y + 1.5 * question_rect.h,
            window_width / 2,
            question_rect.h / 2
    };

    print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(yes_rect), "Bye",
                            game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER);

    rect_t no_rect = {
            window_width / 4,
            question_rect.y + 2.25 * question_rect.h,
            window_width / 2,
            question_rect.h / 2
    };

    print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(no_rect), "I want to stay",
                            game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER);

    display_cursor_gui(game_window, quit ? &yes_rect : &no_rect);

    return EXIT_SUCCESS;
}

int display_confirm_quit_cli(game_window_t * game_window, bool quit) {
    int window_height = 0;
    int window_width = 0;
    cli_get_window_size(&window_width, &window_height);

    cli_render_clear(game_window->matrix, (cli_char_t){' ', game_window->cli_color_palette->background});

    // print title
    rect_t title_rect = {0, 0, window_width, window_height / 2};
    cli_print_text_in_rectangle(game_window->matrix, title_rect, "Bye quitter", game_window->cli_color_palette->text,
                                ALIGN_START, ALIGN_START,SMALL_TEXT);

    // print options
    rect_t option_1_rect = {2, window_height / 2, window_width, window_height / 4};
    rect_t option_2_rect = {2, 3 * (window_height / 4), window_width, window_height / 4};

    cli_print_text_in_rectangle(game_window->matrix, option_1_rect, quit ? "> Bye" : "Bye", game_window->cli_color_palette->text,
                                ALIGN_START, ALIGN_START,SMALL_TEXT);
    cli_print_text_in_rectangle(game_window->matrix, option_2_rect, !quit ? "> I want to stay" : "I want to stay", game_window->cli_color_palette->text,
                                ALIGN_START, ALIGN_START, SMALL_TEXT);

    return EXIT_SUCCESS;
}