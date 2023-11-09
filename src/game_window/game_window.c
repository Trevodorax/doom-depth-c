#include "game_window.h"
#include "../ui_utils/themes/themes.h"
#include <SDL_ttf.h>

game_window_t * init_game_window(ui_type_t ui_type, color_scheme_t color_scheme) {
    game_window_t * game_window = malloc(sizeof(game_window_t));
    if (!game_window) {
        return NULL;
    }

    game_window->context = malloc(sizeof(window_context_t));
    if (!game_window->context) {
        return NULL;
    }

    game_window->ui_type = ui_type;

    // starting here, it is specific per ui types
    switch (ui_type) {
        case CLI: {
            if (init_game_window_cli(game_window) == EXIT_FAILURE) {
                return NULL;
            }
            break;
        }
        case GUI: {
            if (init_game_window_gui(game_window) == EXIT_FAILURE) {
                return NULL;
            }
            break;
        }
    }

    if (init_colors(game_window, color_scheme) == EXIT_FAILURE) {
        return NULL;
    }

    return game_window;
}


int init_game_window_cli(game_window_t * game_window) {
    int cli_width;
    int cli_height;
    cli_get_window_size(&cli_width, &cli_height);

    game_window->matrix = create_cli_matrix(cli_height, cli_width, 0, RED);

    if (game_window->matrix == NULL) {
        return EXIT_FAILURE;
    }

    if(cli_render_clear(game_window->matrix, (cli_char_t){' ', WHITE}) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int init_game_window_gui(game_window_t * game_window) {
    if (SDL_Init(SDL_INIT_VIDEO) || TTF_Init()) {
        return EXIT_FAILURE;
    }

    game_window->window = SDL_CreateWindow(
            "Doom depth c",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            700,
            500,
            SDL_WINDOW_RESIZABLE
    );
    if (!game_window->window) {
        return EXIT_FAILURE;
    }

    game_window->renderer = SDL_CreateRenderer(
            game_window->window,
            1,
            SDL_RENDERER_ACCELERATED
    );
    if (!game_window->renderer) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void free_game_window(game_window_t *game_window, ui_type_t ui_type) {
    switch (ui_type) {
        case CLI:
            free_game_window_cli(game_window);
            break;
        case GUI:
            free_game_window_gui(game_window);
            break;
    }


    if(game_window->context) {
        free(game_window->context);
    }
    free(game_window);
}

void free_game_window_cli(game_window_t * game_window) {
    free_matrix(game_window->matrix);
    system("clear");
}

void free_game_window_gui(game_window_t *game_window) {
    if (game_window->window) {
        SDL_DestroyWindow(game_window->window);
    }
    if (game_window->renderer) {
        SDL_DestroyRenderer(game_window->renderer);
    }

    TTF_Quit();
    SDL_Quit();
}

int render_present_cli(game_window_t * game_window);
int render_present_gui(game_window_t * game_window);
int render_present(game_window_t * game_window) {
    switch (game_window->ui_type) {
        case CLI:
            return render_present_cli(game_window);
        case GUI:
            return render_present_gui(game_window);
    }
}

int render_present_cli(game_window_t * game_window) {
    resize_cli_matrix_to_window(game_window->matrix, (cli_char_t){' ', game_window->cli_color_palette->background});
    set_cli_raw_mode(true);
    cli_render_present(game_window->matrix, game_window->cli_color_palette->text);
    set_cli_raw_mode(false);

    return EXIT_SUCCESS;
}

int render_present_gui(game_window_t * game_window) {
    SDL_RenderPresent(game_window->renderer);
    return EXIT_SUCCESS;
}
