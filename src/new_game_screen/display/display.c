#include "string.h"
#include "display.h"
#include "../../ui_utils/sdl_utils/sdl_utils.h"

int display_new_game(game_window_t *game_window, char *name) {

    switch (game_window->ui_type) {
        case CLI:
            return display_new_game_cli(game_window, name);
        case GUI:
            return display_new_game_gui(game_window, name);
    }

}

int display_new_game_cli(game_window_t *game_window, char *name) {

    int window_width = 0;
    int window_height = 0;

    cli_get_window_size(&window_width, &window_height);

    cli_render_clear(game_window->matrix, (cli_char_t){' ', game_window->cli_color_palette->background});

    // Display welcome message
    rect_t welcome_rect = {0, 0, window_width, window_height / 2};
    cli_print_text_in_rectangle(game_window->matrix, welcome_rect, "Create a new game", game_window->cli_color_palette->text, ALIGN_CENTER, ALIGN_START,
                                MEDIUM_TEXT);

    // Display name question
    rect_t name_question_rect = {0, welcome_rect.y + welcome_rect.h, window_width, window_height / 4};
    cli_print_text_in_rectangle(game_window->matrix, name_question_rect, "Enter your name", game_window->cli_color_palette->text, ALIGN_CENTER, ALIGN_START,
                                SMALL_TEXT);

    // Display name input
    rect_t name_input_rect = {0, name_question_rect.y + name_question_rect.h + 2, window_width, window_height / 4};
    cli_print_text_in_rectangle(game_window->matrix, name_input_rect, name, game_window->cli_color_palette->text, ALIGN_CENTER, ALIGN_START,
                                SMALL_TEXT);

    return EXIT_SUCCESS;

}

int display_new_game_gui(game_window_t *game_window, char *name) {

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    SDL_RenderClear(game_window->renderer);

    draw_fill_rect(
            (SDL_Rect){0, 0, window_width, window_height},
            game_window->sdl_color_palette->background,
            game_window->renderer
    );

    SDL_Rect welcome_rect = {0, 0, 0, 0};
    SDL_Rect name_question_rect = {0, 0, 0, 0};
    SDL_Rect name_input_rect = {0, 0, 0, 0};
    SDL_Rect name_input_cursor_rect = {0, 0, 0, 0};

    display_welcome(game_window, &welcome_rect, window_width, window_height);
    display_name_question(game_window, &name_question_rect, window_width, welcome_rect.y);
    display_name_input(game_window, &name_input_rect, window_width, name_question_rect.y, name);
    if (strlen(name) != 0) {
        display_name_input_cursor(
                game_window,
                &name_input_cursor_rect,
                name_input_rect.x + name_input_rect.w + 8,
                name_input_rect.y
                );
    } else {
        display_name_input_cursor(
                game_window,
                &name_input_cursor_rect,
                name_input_rect.x,
                name_input_rect.y
        );
    }

    return EXIT_SUCCESS;
}

int display_welcome(game_window_t * game_window, SDL_Rect *container, int window_width, int window_height) {
    SDL_Texture *welcome_texture = get_string_texture(
            game_window->renderer,
            "Welcome, new player!",
            "../assets/PixelifySans-Regular.ttf",
            24,
            game_window->sdl_color_palette->text
    );
    if (!welcome_texture) {
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(welcome_texture, NULL, NULL, &(container->w), &(container->h));
    container->x = (window_width - container->w) / 2;
    container->y = (window_height - container->h) / 2 - 100;

    SDL_RenderCopy(game_window->renderer, welcome_texture, NULL, container);
    SDL_DestroyTexture(welcome_texture);

    return EXIT_SUCCESS;
}

int display_name_question(game_window_t * game_window, SDL_Rect *container, int width_ref, int height_ref) {
    SDL_Texture *name_question_texture = get_string_texture(
            game_window->renderer,
            "What is your name?",
            "../assets/PixelifySans-Regular.ttf",
            48,
            game_window->sdl_color_palette->text
    );
    if (!name_question_texture) {
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(name_question_texture, NULL, NULL, &(container->w), &(container->h));
    container->x = (width_ref - container->w) / 2;
    container->y = height_ref + 64;

    SDL_RenderCopy(game_window->renderer, name_question_texture, NULL, container);
    SDL_DestroyTexture(name_question_texture);

    return EXIT_SUCCESS;
}

int display_name_input(game_window_t * game_window, SDL_Rect *container, int width_ref, int height_ref, const char *name) {
    SDL_Texture *name_input_texture = get_string_texture(
            game_window->renderer,
            name,
            "../assets/PixelifySans-Regular.ttf",
            48,
            game_window->sdl_color_palette->text
    );
    if (!name_input_texture) {
        container->x = width_ref / 2;
        container->y = height_ref + 64;
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(name_input_texture, NULL, NULL, &(container->w), &(container->h));
    container->x = (width_ref - container->w) / 2;
    container->y = height_ref + 64;

    SDL_RenderCopy(game_window->renderer, name_input_texture, NULL, container);
    SDL_DestroyTexture(name_input_texture);

    return EXIT_SUCCESS;
}

int display_name_input_cursor(game_window_t * game_window, SDL_Rect *container, int width_ref, int height_ref) {
    SDL_Texture *cursor_texture = get_string_texture(
            game_window->renderer,
            "|",
            "../assets/PixelifySans-Regular.ttf",
            48,
            game_window->sdl_color_palette->text
    );
    if (!cursor_texture) {
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(cursor_texture, NULL, NULL, &(container->w), &(container->h));
    container->x = width_ref;
    container->y = height_ref;

    SDL_RenderCopy(game_window->renderer, cursor_texture, NULL, container);
    SDL_DestroyTexture(cursor_texture);

    return EXIT_SUCCESS;
}