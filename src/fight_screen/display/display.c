#include "display.h"

int display_fight_cli(game_window_t *game_window, fight_context_t * fight_context, rect_t fight_zone);
int display_fight_gui(game_window_t *game_window, fight_context_t * fight_context, SDL_Rect fight_zone);

int display_fight(game_window_t *game_window, fight_context_t * fight_context, rect_t fight_zone) {
    switch(game_window->ui_type) {
        case CLI:
            return display_fight_cli(game_window, fight_context, fight_zone);
        case GUI:
            return display_fight_gui(game_window, fight_context, rect_to_SDL_Rect(fight_zone));
    }
}

int display_fight_cli(game_window_t *game_window, fight_context_t * fight_context, rect_t fight_zone) {
    cli_draw_stroke_rect(game_window->matrix, fight_zone, (cli_char_t){'#', CYAN});
    return EXIT_SUCCESS;
}

int display_fight_gui(game_window_t *game_window, fight_context_t * fight_context, SDL_Rect fight_zone) {
    if (!game_window) {
        fprintf(stderr, "\ndisplay_menu error: Please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    draw_fill_rect(fight_zone, (SDL_Color){0, 180, 50, 255}, game_window->renderer);


    // TODO add notification part
    //  add fight_context->notification_message in top of the screen

    // TODO add fight part

    return EXIT_SUCCESS;
}
