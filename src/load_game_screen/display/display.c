#include "display.h"

int display_title(game_window_t *game_window, int window_width, int window_height, SDL_Color text_color);

int display_load_game(game_window_t *game_window, array_node_t *players, unsigned short active_option) {

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);
    SDL_RenderClear(game_window->renderer);
    SDL_Color text_color = {255, 255, 255, 255};

    display_title(game_window, window_width, window_height, text_color);

    rect_t container_saves = {
            window_width / 4,
            window_height / 4,
            window_width / 2,
            window_height / 2
    };

    rect_t *rects = get_rectangle_layout(3, &container_saves, VERTICAL);
    for (int i = 0; i < 3; i++) {
        player_t *current_player = get_value_at_index(players, i);
        print_text_in_rectangle(
                game_window->renderer,
                rect_to_SDL_Rect(rects[i]),
                current_player != NULL ? current_player->name : "Empty",
                text_color,
                ALIGN_CENTER,
                ALIGN_CENTER);
    }

    rect_t container_cursor = {
            container_saves.x - 20,
            rects[active_option].y + rects[active_option].h / 2,
            40,
            20
    };

    print_text_in_rectangle(
            game_window->renderer,
            rect_to_SDL_Rect(container_cursor),
            ">",
            text_color,
            ALIGN_CENTER,
            ALIGN_CENTER);

    SDL_RenderPresent(game_window->renderer);

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