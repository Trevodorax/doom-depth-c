#include "cli.h"

int display_menu_item_cli(game_window_t * game_window, const char * title, const char * image_path, rect_t container,
                          bool is_selected);


int display_menu_cli(game_window_t *game_window, menu_t * menu, rect_t container, int selected_item_index, bool player_turn) {
    cli_draw_stroke_rect(game_window->matrix, container, (cli_char_t){'#', RED});

    return EXIT_SUCCESS;
}

int display_menu_item_cli(game_window_t * game_window, const char *title, const char *image_path, rect_t container,
                          bool is_selected) {
    return EXIT_SUCCESS;
}
