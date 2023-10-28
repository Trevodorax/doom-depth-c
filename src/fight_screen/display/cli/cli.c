#include "cli.h"

int display_fight_cli(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone) {
    cli_draw_stroke_rect(game_window->matrix, fight_zone, (cli_char_t){'#', CYAN});
    return EXIT_SUCCESS;
}
