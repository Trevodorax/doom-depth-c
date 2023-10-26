#include "display.h"

/**
 * @brief Displays the monsters in the fight
 *
 * @param game_window
 * @param monsters The list of monsters in the fight
 * @param container The zone in which to print them
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_monsters_gui(game_window_t * game_window, array_node_t * monsters, SDL_Rect container);

int display_monster_gui(game_window_t * game_window, monster_t *monster, SDL_Rect container);

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

    draw_fill_rect(fight_zone, (SDL_Color){255, 255, 255, 255}, game_window->renderer);

    SDL_Rect monsters_zone = {
            fight_zone.x + fight_zone.w / 3,
            fight_zone.y,
            fight_zone.w - fight_zone.w / 3,
            fight_zone.h
    };

    display_monsters_gui(game_window, fight_context->monsters, monsters_zone);

    // TODO add notification part
    //  add fight_context->notification_message in top of the screen

    // TODO add fight part

    return EXIT_SUCCESS;
}

int display_monsters_gui(game_window_t * game_window, array_node_t * monsters, SDL_Rect container) {
    size_t nb_monsters = get_size(monsters);

    SDL_Rect * monster_containers = get_rectangle_layout(nb_monsters, &container, HORIZONTAL);
    if(!monster_containers) {
        return EXIT_FAILURE;
    }

    size_t i = 0;
    array_node_t * current_monster = monsters;
    while(current_monster != NULL) {
        monster_t * checked_current_monster = void_to_monster(current_monster->value);
        if(!checked_current_monster) {
            return EXIT_FAILURE;
        }
        display_monster_gui(game_window, checked_current_monster, monster_containers[i++]);
        current_monster = current_monster->next;
    }

    return EXIT_SUCCESS;
}

int display_monster_gui(game_window_t * game_window, monster_t *monster, SDL_Rect container) {
    if (!monster->image_path) {
        return EXIT_FAILURE;
    }
    draw_image_in_rectangle(game_window->renderer, container, monster->image_path, NORTH);

    return EXIT_SUCCESS;
}
