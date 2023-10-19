#include "display.h"
#include "../../../sdl_utils/sdl_utils.h"
#include "../../stage/display/display.h"

int display_map_gui(game_window_t *game_window, map_t *map, SDL_Texture **stage_textures);
int display_map_cli(game_window_t *game_window, map_t *map);

int display_map(game_window_t *game_window, map_t *map, SDL_Texture **stage_textures) {
    switch (game_window->ui_type) {
        case CLI:
            return display_map_cli(game_window, map);
        case GUI:
            return display_map_gui(game_window, map, stage_textures);
    }
}

int display_map_cli(game_window_t *game_window, map_t *map) {
    if (!game_window || !map) {
        fprintf(stderr, "\ndisplay_map_cli error: please provide all necessary arguments");
        return EXIT_FAILURE;
    }

    // process all dimensions
    int window_width = (int)game_window->matrix->nb_cols;
    int window_height = (int)game_window->matrix->nb_rows;

    int map_width;
    int map_height;
    int initial_x;
    int initial_y;
    get_map_dimensions(map, &map_width, &map_height, &initial_x, &initial_y);

    int map_size = map_width > map_height ? map_width : map_height;
    int window_size = window_width < window_height ? window_width : window_height;

    int stage_size = window_size / map_size;

    char background_char = ' ';
    color_code_t background_color = WHITE;
    if (cli_render_clear(game_window->matrix, (cli_char_t){background_char, background_color}) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    print_stages(game_window, map->first_stage, NULL, initial_x, initial_y, stage_size, true);

    return EXIT_SUCCESS;
}

int display_map_gui(game_window_t *game_window, map_t *map, SDL_Texture **stage_textures) {
    if (!game_window || !map) {
        fprintf(stderr, "\ndisplay_map_gui error: please provide all necessary arguments");
        return EXIT_FAILURE;
    }

    // process all dimensions
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    stage_t * test = map->first_stage;

    int map_width;
    int map_height;
    int initial_x;
    int initial_y;
    get_map_dimensions(map, &map_width, &map_height, &initial_x, &initial_y);

    int map_size = map_width > map_height ? map_width : map_height;
    int window_size = window_width < window_height ? window_width : window_height;

    int stage_size = window_size / map_size;

    SDL_Color background_color = { 255, 255, 255, 255};
    if (set_background_color(game_window->renderer, background_color) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    print_stages(game_window, map->first_stage, stage_textures, initial_x, initial_y, stage_size, true);

    return EXIT_SUCCESS;
}