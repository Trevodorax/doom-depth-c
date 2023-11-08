#include "display.h"

int display_title(game_window_t * game_window, int window_width, int window_height, SDL_Color text_color);

int display_load_game(game_window_t *game_window, list_t *players, unsigned short active_option) {

    switch (game_window->ui_type) {
        case CLI:
            return display_load_game_cli(game_window, players, active_option);
        case GUI:
            return display_load_game_gui(game_window, players, active_option);
    }

}

int display_load_game_gui(game_window_t *game_window, list_t *players, unsigned short active_option) {

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);
    SDL_RenderClear(game_window->renderer);

    display_title(game_window, window_width, window_height, game_window->sdl_color_palette->text);

    rect_t container_saves = {
            (size_t)(window_width - window_width * 0.3)/ 2,
            (size_t)(window_height - window_height * 0.3) / 2,
            (size_t)(window_width * 0.3),
            (size_t)(window_height * 0.3)
    };

    rect_t * rects = get_rectangle_layout(3, &container_saves, VERTICAL, 2);
    if (players != NULL) {
        for (int i = 0; i < 3; i++) {
            player_t *current_player = get_value_at_index(players, i);
            print_text_in_rectangle(
                    game_window->renderer,
                    rect_to_SDL_Rect(rects[i]),
                    current_player != NULL ? current_player->name : "Empty",
                    game_window->sdl_color_palette->text,
                    ALIGN_CENTER,
                    ALIGN_CENTER);
        }
    } else {
        for (int i = 0; i < 3; i++) {
            print_text_in_rectangle(
                    game_window->renderer,
                    rect_to_SDL_Rect(rects[i]),
                    "Empty",
                    game_window->sdl_color_palette->text,
                    ALIGN_CENTER,
                    ALIGN_CENTER);
        }
    }

    rect_t container_cursor = {
            rects[active_option].x - (rects[active_option].w / 10),
            rects[active_option].y + (rects[active_option].h / 2) - (rects[active_option].h / 4),
            rects[active_option].w / 10,
            rects[active_option].h / 2
    };

    print_text_in_rectangle(
            game_window->renderer,
            rect_to_SDL_Rect(container_cursor),
            ">",
            game_window->sdl_color_palette->highlight,
            ALIGN_CENTER,
            ALIGN_CENTER);

    return EXIT_SUCCESS;
}

int display_title(game_window_t *game_window, int window_width, int window_height, SDL_Color text_color) {
    rect_t container_title = {
            window_width / 4,
            0,
            window_width / 2,
            window_height / 4
    };

    print_text_in_rectangle(
            game_window->renderer,
            rect_to_SDL_Rect(container_title),
            "Load a game",
            text_color,
            ALIGN_CENTER,
            ALIGN_CENTER);

    return EXIT_SUCCESS;
}

int display_load_game_cli(game_window_t *game_window, list_t *players, unsigned short active_option) {

    int window_height = 0;
    int window_width = 0;
    cli_get_window_size(&window_width, &window_height);

    cli_render_clear(game_window->matrix, (cli_char_t){' ', game_window->cli_color_palette->background});

    // print title
    rect_t title_rect = {0, 0, window_width, window_height / 2};
    cli_print_text_in_rectangle(game_window->matrix, title_rect, "Load a game", game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_START, MEDIUM_TEXT);

    rect_t container_saves = {
            0,
            (size_t)window_height / 2,
            window_width,
            (size_t)(window_height * 0.4)
    };
    rect_t *rects = get_rectangle_layout(3, &container_saves, VERTICAL, 0);

    // print saves
    if (players != NULL) {
        for (int i = 0; i < 3; i++) {
            player_t *current_player = get_value_at_index(players, i);
            cli_print_text_in_rectangle(
                    game_window->matrix,
                    rects[i],
                    current_player != NULL ? current_player->name : "Empty",
                    game_window->cli_color_palette->text,
                    ALIGN_CENTER,
                    ALIGN_CENTER,
                    MEDIUM_TEXT);
        }
    } else {
        for (int i = 0; i < 3; i++) {
            cli_print_text_in_rectangle(
                    game_window->matrix,
                    rects[i],
                    active_option == i ? "> Empty" : "Empty",
                    game_window->cli_color_palette->text,
                    ALIGN_CENTER,
                    ALIGN_CENTER,
                    MEDIUM_TEXT);
        }
    }

    return EXIT_SUCCESS;

}