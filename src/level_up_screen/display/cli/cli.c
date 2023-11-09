#include "cli.h"

int display_level_up_cli(game_window_t *game_window, player_t *player, stat_t selected_stat) {
    cli_render_clear(game_window->matrix, (cli_char_t){'#', RED});

    return EXIT_SUCCESS;
}
