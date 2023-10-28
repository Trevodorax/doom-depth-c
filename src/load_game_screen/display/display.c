#include "display.h"

int *player_rect(game_window_t *game_window, array_node_t **current_player, SDL_Color text_color, int window_width, int window_height, int i, int title_position);
int display_cursor(game_window_t *game_window, int active_option, int window_width, int window_height, int **rects, SDL_Color text_color);
int display_title(game_window_t *game_window, int window_width, int window_height, SDL_Color text_color);

int display_load_game(game_window_t *game_window, array_node_t *players, unsigned short active_option) {

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);
    SDL_RenderClear(game_window->renderer);
    SDL_Color text_color = {255, 255, 255, 255};

    int title_position = display_title(game_window, window_width, window_height, text_color);
    if (title_position == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    array_node_t *current_player = players;
    int i = 0;
    int **rects = malloc(sizeof(int*)*3);
    for (int j = 0; j < 3; j++) {
        rects[i] = player_rect(game_window, &current_player, text_color, window_width, window_height, i, title_position);
        if (rects[i] == NULL) {
            return EXIT_FAILURE;
        }
        i++;
    }

    if(display_cursor(game_window, active_option, window_width, window_height, rects, text_color) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}

int *player_rect(game_window_t *game_window, array_node_t **current_player, SDL_Color text_color, int window_width, int window_height, int i, int title_position) {
    SDL_Texture *player_texture = NULL;
    if(*current_player != NULL) {
        player_texture = get_string_texture(
                game_window->renderer,
                ((player_t *) (*current_player)->value)->name,
                "../assets/PixelifySans-Bold.ttf",
                24,
                text_color
        );
        *current_player = (*current_player)->next;
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
        return NULL;
    }

    int player_width = 0;
    int player_height = 0;
    SDL_QueryTexture(player_texture, NULL, NULL, &player_width, &player_height);


    SDL_Rect player_rect = {
            (window_width - player_width) / 2,
            title_position + 128 + (i * 48),
            player_width,
            player_height
    };

    SDL_RenderCopy(game_window->renderer, player_texture, NULL, &player_rect);
    SDL_DestroyTexture(player_texture);

    int *rect = malloc(sizeof(int)*2);
    rect[0] = player_rect.x;
    rect[1] = player_rect.y;

    return rect;
}

int display_cursor(game_window_t *game_window, int active_option, int window_width, int window_height, int **rects, SDL_Color text_color) {
    SDL_Texture *cursor_texture = get_string_texture(
            game_window->renderer,
            ">",
            "../assets/PixelifySans-Bold.ttf",
            12,
            text_color
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
                rects[0][0] - cursor_width - 8,
                rects[0][1] + cursor_height / 2,
                cursor_width,
                cursor_height
        };
    } else if (active_option == 1) {
        cursor_rect = (SDL_Rect) {
                rects[1][0] - cursor_width - 8,
                rects[1][1] + cursor_height / 2,
                cursor_width,
                cursor_height
        };
    } else {
        cursor_rect = (SDL_Rect) {
                rects[2][0] - cursor_width - 8,
                rects[2][1] + cursor_height / 2,
                cursor_width,
                cursor_height
        };
    }

    // add cursor to renderer
    SDL_RenderCopy(game_window->renderer, cursor_texture, NULL, &cursor_rect);
    SDL_DestroyTexture(cursor_texture);

    return EXIT_SUCCESS;
}

int display_title(game_window_t *game_window, int window_width, int window_height, SDL_Color text_color) {
    SDL_Texture *title_texture = get_string_texture(
            game_window->renderer,
            "Charger une partie",
            "../assets/PixelifySans-Bold.ttf",
            48,
            text_color
    );
    if (!title_texture) {
        return EXIT_FAILURE;
    }

    int title_width = 0;
    int title_height = 0;
    SDL_QueryTexture(title_texture, NULL, NULL, &title_width, &title_height);

    SDL_Rect title_rect = {
            (window_width - title_width) / 2,
            (window_height - title_height) / 2 - 100,
            title_width,
            title_height
    };

    SDL_RenderCopy(game_window->renderer, title_texture, NULL, &title_rect);
    SDL_DestroyTexture(title_texture);

    return title_rect.y;
}