#include "fight_dimensions.h"

void update_fight_section_dimensions(game_window_t * game_window, rect_t * fight_zone, rect_t *menu_zone) {
    // get the dimensions of window
    int window_height = 0;
    int window_width = 0;

    switch(game_window->ui_type) {
        case CLI:
            resize_cli_matrix_to_window(game_window->matrix, (cli_char_t){' ', WHITE});
            window_height = (int)game_window->matrix->nb_rows;
            window_width = (int)game_window->matrix->nb_cols;
        case GUI:
            SDL_GetWindowSize(game_window->window, &window_width, &window_height);
    }

    // use them to define zones for different zones
    fight_zone->x = 0;
    fight_zone->y = 0;
    fight_zone->w = window_width;
    fight_zone->h = window_height / 2;

    menu_zone->x = 0;
    menu_zone->y = window_height / 2;
    menu_zone->w = window_width;
    menu_zone->h = window_height / 2;
}
