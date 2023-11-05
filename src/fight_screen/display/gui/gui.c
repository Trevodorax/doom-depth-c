#include "gui.h"
#include "../../../ui_utils/ui_utils.h"

int display_monsters_gui(game_window_t * game_window, array_node_t * monsters, rect_t container);
int display_monster_gui(game_window_t * game_window, monster_t *monster, SDL_Rect container);
int display_player_in_fight_gui(game_window_t * game_window, player_t * player, SDL_Rect container);
int display_notification_zone_gui(game_window_t * game_window, char * notification_text, SDL_Rect container);
int display_stat_bar_gui(game_window_t *game_window, int current_stat, int max_stat, SDL_Rect container,
                         SDL_Color filled_color, SDL_Color empty_color);
int display_player_stats_zone_gui(game_window_t * game_window, player_t * player, SDL_Rect container);

int display_fight_gui(game_window_t * game_window, fight_context_t * fight_context, SDL_Rect fight_zone) {
    if (!game_window) {
        global_logger->error("\ndisplay_menu error: Please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    draw_fill_rect(fight_zone, (SDL_Color){255, 255, 255, 255}, game_window->renderer);

    bool has_notification_message = fight_context->notification_message && strlen(fight_context->notification_message) > 0;

    int player_stats_zone_height = 32;
    int notification_zone_height = has_notification_message ? 32 : 0;

    rect_t player_stats_zone = {
            fight_zone.x,
            fight_zone.y,
            fight_zone.w,
            player_stats_zone_height
    };

    rect_t player_zone = {
            fight_zone.x,
            fight_zone.y + player_stats_zone_height,
            fight_zone.w / 3,
            fight_zone.h - player_stats_zone_height - notification_zone_height
    };

    rect_t monsters_zone = {
            fight_zone.x + player_zone.w,
            fight_zone.y + player_stats_zone_height,
            fight_zone.w - player_zone.w,
            fight_zone.h - notification_zone_height - player_stats_zone_height
    };

    rect_t notification_zone = {
            fight_zone.x,
            fight_zone.h - notification_zone_height,
            fight_zone.w,
            notification_zone_height
    };

    display_player_stats_zone_gui(game_window, fight_context->player, rect_to_SDL_Rect(player_stats_zone));
    display_player_in_fight_gui(game_window, fight_context->player, rect_to_SDL_Rect(player_zone));
    if (has_notification_message) {
        display_notification_zone_gui(game_window, fight_context->notification_message, rect_to_SDL_Rect(notification_zone));
    }
    display_monsters_gui(game_window, fight_context->monsters, monsters_zone);

    return EXIT_SUCCESS;
}

int display_monsters_gui(game_window_t * game_window, array_node_t * monsters, rect_t container) {
    size_t nb_monsters = get_size(monsters);

    rect_t * monster_containers = get_rectangle_layout(nb_monsters, &container, HORIZONTAL, 2);
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
        display_monster_gui(game_window, checked_current_monster, rect_to_SDL_Rect(monster_containers[i++]));

        current_monster = current_monster->next;
    }

    return EXIT_SUCCESS;
}

int display_monster_gui(game_window_t * game_window, monster_t *monster, SDL_Rect container) {
    if (!monster->image_path) {
        return EXIT_FAILURE;
    }

    int hp_rect_height = container.h / 10;
    if (hp_rect_height > 16) {
        hp_rect_height = 16;
    }

    SDL_Rect monster_rect = {
            container.x,
            container.y,
            container.w,
            container.h - hp_rect_height - 4
    };

    SDL_Rect hp_rect = {
            container.x,
            container.y + container.h - hp_rect_height,
            container.w,
            hp_rect_height
    };

    if(draw_image_in_rectangle(game_window->renderer, monster_rect, monster->image_path, NORTH, true, ALIGN_CENTER,
                               ALIGN_END) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if(display_stat_bar_gui(game_window, (int) monster->hp, (int) monster->hp_max, hp_rect,
                            (SDL_Color) {50, 200, 50, 255}, (SDL_Color) {200, 50, 50, 255}) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int display_player_in_fight_gui(game_window_t * game_window, player_t * player, SDL_Rect container) {
    int container_padding = (int)container.w / 10;

    container.x += container_padding;
    container.y += container_padding;
    container.w -= container_padding * 2;
    container.h -= container_padding * 2;

    SDL_Rect player_rect = container;

    if(draw_image_in_rectangle(game_window->renderer, player_rect, "../assets/player/image/player.png", EAST, true, ALIGN_CENTER, ALIGN_END) == EXIT_FAILURE) {
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

int display_stat_bar_gui(game_window_t *game_window, int current_stat, int max_stat, SDL_Rect container,
                         SDL_Color filled_color, SDL_Color empty_color) {
    int stat_left_width = (int) (((double)current_stat / max_stat) * container.w);

    SDL_Rect stat_left_rect = {
            container.x,
            container.y,
            stat_left_width,
            container.h
    };

    SDL_Rect stat_removed_rect = {
            container.x + stat_left_width,
            container.y,
            container.w - stat_left_width,
            container.h
    };

    if(draw_fill_rect(stat_left_rect, filled_color, game_window->renderer) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if(draw_fill_rect(stat_removed_rect, empty_color, game_window->renderer) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_player_stats_zone_gui(game_window_t * game_window, player_t * player, SDL_Rect container) {
    if (draw_fill_rect(container, (SDL_Color){200, 200, 200, 255}, game_window->renderer) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    // apply padding
    int container_padding_x = container.w / 10 > 32 ? 32 : container.w / 10;
    int container_padding_y = container.h / 10 > 32 ? 32 : container.h / 5;
    container.x += container_padding_x;
    container.y += container_padding_y;
    container.w -= container_padding_x * 2;
    container.h -= container_padding_y * 2;

    // set and process dimensions
    int space_between_stats = 4;
    int nb_stats = 3;
    int stat_width = container.w / nb_stats - space_between_stats;

    SDL_Rect hp_rect = {
            container.x,
            container.y,
            stat_width,
            container.h
    };
    SDL_Rect mana_rect = {
            container.x + stat_width + space_between_stats,
            container.y,
            stat_width,
            container.h
    };

    SDL_Rect actions_points_rect = {
            container.x + 2 * (stat_width + space_between_stats),
            container.y,
            stat_width,
            container.h
    };

    if(display_stat_bar_gui(
            game_window,
            (int)player->hp,
            (int)player->hp_max,
            hp_rect,
            (SDL_Color) {50, 200, 50, 255},
            (SDL_Color) {200, 50, 50, 255}
    ) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    print_text_in_rectangle(game_window->renderer, hp_rect, "HP", (SDL_Color){0, 0, 0, 255}, ALIGN_CENTER, ALIGN_CENTER);

    if(display_stat_bar_gui(
            game_window,
            (int)player->mana,
            (int)player->mana_max,
            mana_rect,
            (SDL_Color) {100, 100, 200, 255},
            (SDL_Color) {50, 50, 50, 255}
    ) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    print_text_in_rectangle(game_window->renderer, mana_rect, "MANA", (SDL_Color){255, 255, 255, 255}, ALIGN_CENTER, ALIGN_CENTER);


    if(display_stat_bar_gui(
            game_window,
            (int)player->action_points,
            (int)player->max_action_points,
            actions_points_rect,
            (SDL_Color) {200, 200, 50, 255},
            (SDL_Color) {50, 50, 50, 255}
    ) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    print_text_in_rectangle(game_window->renderer, actions_points_rect, "PA", (SDL_Color){0, 0, 0, 255}, ALIGN_CENTER, ALIGN_CENTER);


    return EXIT_SUCCESS;
}
