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
    SDL_Color grey = {128, 128, 128, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color green = {0, 255, 0, 255};
    SDL_Color blue = {0, 0, 255, 255};
    SDL_Color cyan = {0, 255, 255, 255};
    SDL_Color magenta = {255, 0, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    color_palette->disabled = grey;
    color_palette->black = black;
    color_palette->white = white;
    color_palette->red = red;
    color_palette->green = green;
    color_palette->blue = blue;
    color_palette->cyan = cyan;
    color_palette->magenta = magenta;
    color_palette->yellow = yellow;

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

    // additional colors
    color_palette->white80 = (SDL_Color) {20, 20, 20, 255};
    color_palette->purple = (SDL_Color) {66, 22, 144, 255};
    color_palette->purple80 = (SDL_Color) {46, 14, 102, 255};
}

void init_colors_cli(cli_color_palette_t * color_palette, color_scheme_t color_scheme) {
    color_palette->disabled = BLACK;
    color_palette->black = BLACK;
    color_palette->white = WHITE;
    color_palette->red = RED;
    color_palette->green = GREEN;
    color_palette->blue = BLUE;
    color_palette->cyan = CYAN;
    color_palette->magenta = MAGENTA;
    color_palette->yellow = YELLOW;

    switch (color_scheme) {
        case LIGHT:
            color_palette->background = WHITE;
            color_palette->text = BLACK;
            break;

        case DARK:
            color_palette->background = BLACK;
            color_palette->text = WHITE;
            break;

        default:
            break;
    }
    color_palette->highlight = RED;
}