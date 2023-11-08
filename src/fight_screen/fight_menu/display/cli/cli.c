#include "cli.h"
#include "../../../../ui_utils/ui_utils.h"
#include "../../../../ui_utils/cli_utils/ascii_art/ascii_art.h"

int display_menu_item_cli(game_window_t * game_window, const char * title, const char * ascii_art_path, rect_t container, bool is_selected);

int display_menu_cli(game_window_t * game_window, menu_t *menu, rect_t container, int selected_item_index, bool player_turn) {
    if (!game_window || !menu || selected_item_index >= menu->nb_options || selected_item_index < 0) {
        global_logger->error("\ndisplay_menu error: Please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    char bg_char = player_turn ? ' ' : '.';
    cli_draw_fill_rect(game_window->matrix, container, (cli_char_t){bg_char, game_window->cli_color_palette->background});

    rect_t * menu_items_grid = get_rectangle_grid(menu->nb_options, &container);
    for (int i = 0; i < menu->nb_options; i++) {
        if (display_menu_item_cli(game_window, menu->options[i]->title, menu->options[i]->ascii_art_path, menu_items_grid[i],
                                  i == selected_item_index) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }
    free(menu_items_grid);

    return EXIT_SUCCESS;
}

int display_menu_item_cli(game_window_t * game_window, const char * title, const char * ascii_art_path, rect_t container, bool is_selected) {
    // safeguards
    if (!game_window || (!title && !ascii_art_path)) {
        global_logger->error("\ndisplay_menu_item error: please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    char bg_char = is_selected ? '>' : ' ';
    cli_draw_fill_rect(game_window->matrix, container, (cli_char_t){bg_char, game_window->cli_color_palette->cyan});

    // print the ascii art if there is one
    if (ascii_art_path && strlen(ascii_art_path) > 0) {
        print_ascii_art_in_rectangle(game_window->matrix, ascii_art_path, container, ALIGN_CENTER, ALIGN_CENTER);
    }

    // print the title if there is one
    if (title) {
        cli_print_text_in_rectangle(game_window->matrix, container, title, game_window->cli_color_palette->text,
                                    ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
    }

    return EXIT_SUCCESS;
}
