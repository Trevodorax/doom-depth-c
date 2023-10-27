#include "display.h"

// GUI functions to display the fight elements
int display_monsters_gui(game_window_t * game_window, array_node_t * monsters, SDL_Rect container);
int display_monster_gui(game_window_t * game_window, monster_t *monster, SDL_Rect container);
int display_player_in_fight_gui(game_window_t * game_window, player_t * player, SDL_Rect container);
int display_notification_zone_gui(game_window_t * game_window, char * notification_text, SDL_Rect container);
int display_hp_bar_gui(game_window_t * game_window, int current_hp, int max_hp, SDL_Rect container);

int display_fight_cli(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone);
int display_fight_gui(game_window_t * game_window, fight_context_t * fight_context, SDL_Rect fight_zone);
int display_fight(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone) {
    switch(game_window->ui_type) {
        case CLI:
            return display_fight_cli(game_window, fight_context, fight_zone);
        case GUI:
            return display_fight_gui(game_window, fight_context, rect_to_SDL_Rect(fight_zone));
    }
}

int display_fight_cli(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone) {
    cli_draw_stroke_rect(game_window->matrix, fight_zone, (cli_char_t){'#', CYAN});
    return EXIT_SUCCESS;
}

int display_fight_gui(game_window_t * game_window, fight_context_t * fight_context, SDL_Rect fight_zone) {
    if (!game_window) {
        fprintf(stderr, "\ndisplay_menu error: Please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    draw_fill_rect(fight_zone, (SDL_Color){255, 255, 255, 255}, game_window->renderer);

    int notification_zone_height = 32;

    SDL_Rect player_zone = {
            fight_zone.x,
            fight_zone.y,
            fight_zone.w / 3,
            fight_zone.h - notification_zone_height
    };

    SDL_Rect monsters_zone = {
            fight_zone.x + player_zone.w,
            fight_zone.y,
            fight_zone.w - player_zone.w,
            fight_zone.h - notification_zone_height
    };

    SDL_Rect notification_zone = {
            0,
            fight_zone.h - notification_zone_height,
            fight_zone.w,
            notification_zone_height
    };

    display_player_in_fight_gui(game_window, fight_context->player, player_zone);
    display_monsters_gui(game_window, fight_context->monsters, monsters_zone);
    display_notification_zone_gui(game_window, fight_context->notification_message, notification_zone);

    // TODO add notification part
    //  add fight_context->notification_message in top of the screen

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

    int hp_rect_height = 16;

    SDL_Rect monster_rect = {
            container.x,
            container.y,
            container.w,
            container.h - hp_rect_height - 4
    };

    SDL_Rect hp_rect = {
            container.x,
            container.h - hp_rect_height,
            container.w,
            hp_rect_height
    };

    if(draw_image_in_rectangle(game_window->renderer, monster_rect, monster->image_path, NORTH, true, ALIGN_CENTER,
                               ALIGN_END) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if(display_hp_bar_gui(game_window, (int)monster->hp, (int)monster->hp_max, hp_rect) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_player_in_fight_gui(game_window_t * game_window, player_t * player, SDL_Rect container) {
    int container_padding = 16;

    container.x += container_padding;
    container.y += container_padding;
    container.w -= container_padding * 2;
    container.h -= container_padding * 2;

    int hp_rect_height = 16;

    SDL_Rect player_rect = {
            container.x,
            container.y,
            container.w,
            container.h - hp_rect_height - 16
    };

    SDL_Rect hp_rect = {
            container.x,
            container.h - hp_rect_height,
            container.w,
            hp_rect_height
    };

    if(draw_image_in_rectangle(game_window->renderer, player_rect, "../assets/player/image/player.png", EAST, true, ALIGN_CENTER, ALIGN_END) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if(display_hp_bar_gui(game_window, (int)player->hp, (int)player->hp_max, hp_rect) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_notification_zone_gui(game_window_t * game_window, char * notification_text, SDL_Rect container) {
    if(draw_fill_rect(container, (SDL_Color){200, 200, 200, 255}, game_window->renderer) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if(!notification_text) {
        return EXIT_SUCCESS;
    }

    int padding_x = container.w / 5;
    int padding_y = container.h / 5;

    SDL_Rect text_container = {
        container.x + padding_x,
        container.y + padding_y,
        container.w  - padding_x * 2,
        container.h - padding_y * 2
    };

    print_text_in_rectangle(game_window->renderer, text_container, notification_text, (SDL_Color){0, 0, 0, 255}, ALIGN_CENTER, ALIGN_CENTER);

    return EXIT_SUCCESS;
}

int display_hp_bar_gui(game_window_t * game_window, int current_hp, int max_hp, SDL_Rect container) {
    int hp_left_width = (int) (((double)current_hp / max_hp) * container.w);

    SDL_Rect hp_left_rect = {
            container.x,
            container.y,
            hp_left_width,
            container.h
    };

    SDL_Rect hp_removed_rect = {
            container.x + hp_left_width,
            container.y,
            container.w - hp_left_width,
            container.h
    };

    if(draw_fill_rect(hp_left_rect, (SDL_Color){50, 200, 50, 255}, game_window->renderer) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if(draw_fill_rect(hp_removed_rect, (SDL_Color){200, 50, 50, 255}, game_window->renderer) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
