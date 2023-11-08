#include "cli.h"
#include "../../../ui_utils/ui_utils.h"
#include "../../../ui_utils/cli_utils/ascii_art/ascii_art.h"

int display_monsters_cli(game_window_t * game_window, array_node_t * monsters, rect_t container);
int display_monster_cli(game_window_t * game_window, monster_t *monster, rect_t container);
int display_player_in_fight_cli(game_window_t * game_window, player_t * player, rect_t container);
int display_notification_zone_cli(game_window_t * game_window, char * notification_text, rect_t container);
int display_stat_bar_cli(game_window_t *game_window, int current_stat, int max_stat, rect_t container,
                         color_code_t filled_color, color_code_t empty_color);
int display_player_stats_zone_cli(game_window_t * game_window, player_t * player, rect_t container);

int display_fight_cli(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone) {
    if (!game_window || !fight_context) {
        global_logger->error("\ndisplay_menu error: Please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    cli_draw_fill_rect(game_window->matrix, fight_zone, (cli_char_t){' ', game_window->cli_color_palette->background});

    bool has_notification_message = fight_context->notification_message && strlen(fight_context->notification_message) > 0;

    int player_stats_zone_height = (int)fight_zone.h / 10;
    if (player_stats_zone_height > 3) {
        player_stats_zone_height = 3;
    }
    if (player_stats_zone_height < 1) {
        player_stats_zone_height = 1;
    }

    int notification_zone_height = (int)fight_zone.h / 10;
    if (notification_zone_height > 3) {
        notification_zone_height = 3;
    }
    if (notification_zone_height < 1) {
        notification_zone_height = 1;
    }

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

    display_player_stats_zone_cli(game_window, fight_context->player, player_stats_zone);
    display_player_in_fight_cli(game_window, fight_context->player, player_zone);
    if (has_notification_message) {
        display_notification_zone_cli(game_window, fight_context->notification_message, notification_zone);
    }
    display_monsters_cli(game_window, fight_context->monsters, monsters_zone);

    return EXIT_SUCCESS;
}

int display_monsters_cli(game_window_t * game_window, array_node_t * monsters, rect_t container) {
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
        display_monster_cli(game_window, checked_current_monster, monster_containers[i++]);

        current_monster = current_monster->next;
    }

    return EXIT_SUCCESS;
}

int display_monster_cli(game_window_t * game_window, monster_t *monster, rect_t container) {
    if (!monster->image_path) {
        return EXIT_FAILURE;
    }

    int hp_rect_height = (int)container.h / 10;
    if (hp_rect_height > 3) {
        hp_rect_height = 3;
    }
    if (hp_rect_height < 1) {
        hp_rect_height = 1;
    }

    rect_t monster_rect = {
            container.x,
            container.y,
            container.w,
            container.h - hp_rect_height - 4
    };

    rect_t hp_rect = {
            container.x,
            container.y + container.h - hp_rect_height,
            container.w,
            hp_rect_height
    };

    print_ascii_art_in_rectangle(game_window->matrix, monster->ascii_path, monster_rect, ALIGN_CENTER, ALIGN_CENTER);

    if(display_stat_bar_cli(game_window, (int) monster->hp, (int) monster->hp_max, hp_rect,
                            game_window->cli_color_palette->green, game_window->cli_color_palette->red) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int display_player_in_fight_cli(game_window_t * game_window, player_t * player, rect_t container) {
    int container_padding = (int)container.w / 20;

    container.x += container_padding;
    container.y += container_padding;
    container.w -= container_padding * 2;
    container.h -= container_padding * 2;

    rect_t player_rect = container;

    print_ascii_art_in_rectangle(game_window->matrix, "../assets/player/ascii/player.asciiart", player_rect,
                                 ALIGN_CENTER, ALIGN_CENTER);

    return EXIT_SUCCESS;
}

int display_notification_zone_cli(game_window_t * game_window, char * notification_text, rect_t container) {
    if(cli_draw_fill_rect(game_window->matrix, container, (cli_char_t){' ', game_window->cli_color_palette->white}) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if(!notification_text) {
        return EXIT_SUCCESS;
    }

    int padding_x = (int)container.w / 5;
    int padding_y = (int)container.h / 5;

    rect_t text_container = {
            container.x + padding_x,
            container.y + padding_y,
            container.w  - padding_x * 2,
            container.h - padding_y * 2
    };

    cli_print_text_in_rectangle(game_window->matrix, text_container, notification_text, game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);

    return EXIT_SUCCESS;
}

int display_stat_bar_cli(game_window_t *game_window, int current_stat, int max_stat, rect_t container,
                         color_code_t filled_color, color_code_t empty_color) {
    int stat_left_width = (int) (((double)current_stat / max_stat) * container.w);

    rect_t stat_left_rect = {
            container.x,
            container.y,
            stat_left_width,
            container.h
    };

    rect_t stat_removed_rect = {
            container.x + stat_left_width,
            container.y,
            container.w - stat_left_width,
            container.h
    };

    if(cli_draw_fill_rect(game_window->matrix, stat_left_rect, (cli_char_t){'=', filled_color}) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if(cli_draw_fill_rect(game_window->matrix, stat_removed_rect, (cli_char_t){'=', empty_color}) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_player_stats_zone_cli(game_window_t * game_window, player_t * player, rect_t container) {
    cli_draw_fill_rect(game_window->matrix, container, (cli_char_t){' ', game_window->cli_color_palette->white});

    // apply padding
    int container_padding_x = container.w / 10 > 5 ? 5 : (int)container.w / 10;
    int container_padding_y = container.h / 5 > 5 ? 5 : (int)container.h / 5;
    container.x += container_padding_x;
    container.y += container_padding_y;
    container.w -= container_padding_x * 2;
    container.h -= container_padding_y * 2;

    // set and process dimensions
    int space_between_stats = container.w / 10 > 3 ? 3 : (int)container.w / 10;
    int nb_stats = 3;
    int stat_width = (int)container.w / nb_stats - space_between_stats;

    rect_t hp_rect = {
            container.x,
            container.y,
            stat_width,
            container.h
    };

    rect_t mana_rect = {
            container.x + stat_width + space_between_stats,
            container.y,
            stat_width,
            container.h
    };

    rect_t actions_points_rect = {
            container.x + 2 * (stat_width + space_between_stats),
            container.y,
            stat_width,
            container.h
    };

    if(display_stat_bar_cli(
            game_window,
            (int)player->hp,
            (int)player->hp_max,
            hp_rect,
            game_window->cli_color_palette->green,
            game_window->cli_color_palette->red
    ) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    cli_print_text_in_rectangle(game_window->matrix, hp_rect, "HP", game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);

    if(display_stat_bar_cli(
            game_window,
            (int)player->mana,
            (int)player->mana_max,
            mana_rect,
            game_window->cli_color_palette->blue,
            game_window->cli_color_palette->black
    ) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    cli_print_text_in_rectangle(game_window->matrix, mana_rect, "MANA", game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);

    if(display_stat_bar_cli(
            game_window,
            (int)player->action_points,
            (int)player->max_action_points,
            actions_points_rect,
            game_window->cli_color_palette->yellow,
            game_window->cli_color_palette->black
    ) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    cli_print_text_in_rectangle(game_window->matrix, actions_points_rect, "AP", game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);

    return EXIT_SUCCESS;
}

