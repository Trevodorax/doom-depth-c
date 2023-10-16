#include "string.h"
#include "display.h"
#include "../../sdl_utils/sdl_utils.h"

int display_new_game(game_window_t *game_window, char *name) {

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    SDL_RenderClear(game_window->renderer);

    draw_fill_rect(
            (SDL_Rect){0, 0, window_width, window_height},
            (SDL_Color){0, 0, 0, 255},
            game_window->renderer
    );

    SDL_Rect welcome_rect = {0, 0, 0, 0};
    SDL_Rect name_question_rect = {0, 0, 0, 0};
    SDL_Rect name_input_rect = {0, 0, 0, 0};
    SDL_Rect name_input_cursor_rect = {0, 0, 0, 0};

    display_welcome(game_window->renderer, &welcome_rect, window_width, window_height);
    display_name_question(game_window->renderer, &name_question_rect, window_width, welcome_rect.y);
    display_name_input(game_window->renderer, &name_input_rect, window_width, name_question_rect.y, name);
    if (strlen(name) != 0) {
        display_name_input_cursor(
                game_window->renderer,
                &name_input_cursor_rect,
                name_input_rect.x + name_input_rect.w + 8,
                name_input_rect.y
                );
    } else {
        display_name_input_cursor(
                game_window->renderer,
                &name_input_cursor_rect,
                name_input_rect.x,
                name_input_rect.y
        );
    }

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}

int display_welcome(SDL_Renderer *renderer, SDL_Rect *container, int window_width, int window_height) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};

    SDL_Texture *welcome_texture = get_string_texture(
            renderer,
            "Welcome, new player!",
            "../assets/PixelifySans-Regular.ttf",
            24,
            white
    );
    if (!welcome_texture) {
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(welcome_texture, NULL, NULL, &(container->w), &(container->h));
    container->x = (window_width - container->w) / 2;
    container->y = (window_height - container->h) / 2 - 100;

    SDL_RenderCopy(renderer, welcome_texture, NULL, container);
    SDL_DestroyTexture(welcome_texture);

    return EXIT_SUCCESS;
}

int display_name_question(SDL_Renderer *renderer, SDL_Rect *container, int width_ref, int height_ref) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};

    SDL_Texture *name_question_texture = get_string_texture(
            renderer,
            "What is your name?",
            "../assets/PixelifySans-Regular.ttf",
            48,
            white
    );
    if (!name_question_texture) {
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(name_question_texture, NULL, NULL, &(container->w), &(container->h));
    container->x = (width_ref - container->w) / 2;
    container->y = height_ref + 64;

    SDL_RenderCopy(renderer, name_question_texture, NULL, container);
    SDL_DestroyTexture(name_question_texture);

    return EXIT_SUCCESS;
}

int display_name_input(SDL_Renderer *renderer, SDL_Rect *container, int width_ref, int height_ref, const char *name) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};

    SDL_Texture *name_input_texture = get_string_texture(
            renderer,
            name,
            "../assets/PixelifySans-Regular.ttf",
            48,
            white
    );
    if (!name_input_texture) {
        container->x = width_ref / 2;
        container->y = height_ref + 64;
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(name_input_texture, NULL, NULL, &(container->w), &(container->h));
    container->x = (width_ref - container->w) / 2;
    container->y = height_ref + 64;

    SDL_RenderCopy(renderer, name_input_texture, NULL, container);
    SDL_DestroyTexture(name_input_texture);

    return EXIT_SUCCESS;
}

int display_name_input_cursor(SDL_Renderer *renderer, SDL_Rect *container, int width_ref, int height_ref) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};

    SDL_Texture *cursor_texture = get_string_texture(
            renderer,
            "|",
            "../assets/PixelifySans-Regular.ttf",
            48,
            white
    );
    if (!cursor_texture) {
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(cursor_texture, NULL, NULL, &(container->w), &(container->h));
    container->x = width_ref;
    container->y = height_ref;

    SDL_RenderCopy(renderer, cursor_texture, NULL, container);
    SDL_DestroyTexture(cursor_texture);

    return EXIT_SUCCESS;
}