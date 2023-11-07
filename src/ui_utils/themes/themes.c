#include "themes.h"
#include "../../logs/log.h"

int init_colors(game_window_t * game_window, color_scheme_t color_scheme) {
    game_window->sdl_color_palette = malloc(sizeof(sdl_color_palette_t));
    if (!game_window->sdl_color_palette) {
        global_logger->error("\ninit_colors error: could not allocate sdl_color_palette");
        return EXIT_FAILURE;
    }
    init_colors_gui(game_window->sdl_color_palette, color_scheme);

    game_window->cli_color_palette = malloc(sizeof(cli_color_palette_t));
    if (!game_window->cli_color_palette) {
        global_logger->error("\ninit_colors error: could not allocate cli_color_palette");
        return EXIT_FAILURE;
    }
    init_colors_cli(game_window->cli_color_palette, color_scheme);

    return EXIT_SUCCESS;
}

void init_colors_gui(sdl_color_palette_t * color_palette, color_scheme_t color_scheme) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color red = {255, 0, 0, 255};

    switch (color_scheme) {
        case LIGHT:
            color_palette->background = white;
            color_palette->text = black;
            break;

        case DARK:
            color_palette->background = black;
            color_palette->text = white;
            break;

        default:
            break;
    }
    color_palette->highlight = red;
}

void init_colors_cli(cli_color_palette_t * color_palette, color_scheme_t color_scheme) {
    switch (color_scheme) {
        case LIGHT:
            color_palette->text = BLACK;
            break;

        case DARK:
            color_palette->text = WHITE;
            break;

        default:
            break;
    }
    color_palette->highlight = RED;
}