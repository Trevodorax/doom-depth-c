#include "display.h"

int display_load_game(game_window_t *game_window, array_node_t *players, unsigned short active_option) {

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    SDL_RenderClear(game_window->renderer);

    SDL_Color text_color = {255, 255, 255, 255};

    array_node_t *current_player = players;
    SDL_Texture *player_texture = NULL;
    int player_rect_x = 0;
    for (int i = 0; i < 3; i++) {
        if(current_player != NULL) {
            player_texture = get_string_texture(
                    game_window->renderer,
                    ((player_t *) current_player->value)->name,
                    "../assets/PixelifySans-Bold.ttf",
                    24,
                    text_color
            );
            current_player = current_player->next;
        } else {
            player_texture = get_string_texture(
                    game_window->renderer,
                    "Vide",
                    "../assets/PixelifySans-Bold.ttf",
                    24,
                    text_color
            );
        }

        if (!player_texture) {
            return EXIT_FAILURE;
        }

        int player_width = 0;
        int player_height = 0;
        SDL_QueryTexture(player_texture, NULL, NULL, &player_width, &player_height);

        player_rect_x = (window_width - player_width) / 2;
        SDL_Rect player_rect = {
                player_rect_x,
                (window_height / 2 - 100) + i * 100,
                player_width,
                player_height
        };

        SDL_RenderCopy(game_window->renderer, player_texture, NULL, &player_rect);
        SDL_DestroyTexture(player_texture);
    }

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
    if (active_option == 0) {
        cursor_rect = (SDL_Rect) {
                player_rect_x - cursor_width - 8,
                (window_height / 2 - 100) + 0 * 100 + cursor_height / 2,
                cursor_width,
                cursor_height
        };
    } else if (active_option == 1) {
        cursor_rect = (SDL_Rect) {
                player_rect_x - cursor_width - 8,
                (window_height / 2 - 100) + 1 * 100 + cursor_height / 2,
                cursor_width,
                cursor_height
        };
    } else {
        cursor_rect = (SDL_Rect) {
                player_rect_x - cursor_width - 8,
                (window_height / 2 - 100) + 2 * 100 + cursor_height / 2,
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