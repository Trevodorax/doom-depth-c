#include "display.h"

int display_load_game(game_window_t *game_window, array_node_t *players) {

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    SDL_RenderClear(game_window->renderer);

    rect_t container = {0, 0, window_width, window_height};
    rect_t *rects = get_rectangle_layout(3, &container, VERTICAL);

    SDL_Color text_color = {255, 255, 255, 255};

    array_node_t *current_player = players;
    for (int i = 0; i < 3; i++) {
        if(current_player != NULL) {
            player_t *player = (player_t *) current_player->value;
            print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(rects[i]), player->name, text_color, ALIGN_CENTER, ALIGN_CENTER);
            current_player = current_player->next;
        } else {
            print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(rects[i]), "Aucune sauvegarde", text_color, ALIGN_CENTER, ALIGN_CENTER);
        }
    }

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}