#include "cli.h"
#include "../../../ui_utils/ui_utils.h"
#include "../../../ui_utils/cli_utils/ascii_art/ascii_art.h"

int display_stat_options_cli(game_window_t * game_window, rect_t container, stat_t selected_stat, player_t * player);
int display_stat_option_cli(game_window_t * game_window, unsigned int amount, char * display_name, char * ascii_path, rect_t container, bool is_selected);

int display_level_up_cli(game_window_t * game_window, player_t * player, stat_t selected_stat) {
    cli_render_clear(game_window->matrix, (cli_char_t){' ', WHITE});

    int window_width = 0;
    int window_height = 0;
    cli_get_window_size(&window_width, &window_height);
    resize_cli_matrix(game_window->matrix, window_height, window_width, ' ', WHITE);

    char * title_text = calloc(100, sizeof(char));
    snprintf(title_text, 100, "Level up   %u to %u", player->level, player->level + 1);

    int window_x_padding = window_width / 10;
    int window_y_padding = window_height / 10;

    rect_t container = {
            window_x_padding,
            window_y_padding,
            window_width - 2 * window_x_padding,
            window_height - 2 * window_y_padding
    };

    int title_bottom_padding = window_height / 10;

    rect_t title_container = {
            container.x,
            container.y,
            container.w,
            container.h / 5
    };

    rect_t stats_container = {
            container.x,
            title_container.y + title_container.h + title_bottom_padding,
            container.w,
            container.h - title_container.h - title_bottom_padding
    };

    cli_print_text_in_rectangle(game_window->matrix, title_container, title_text, BLACK, ALIGN_CENTER, ALIGN_START, MEDIUM_TEXT);

    display_stat_options_cli(game_window, stats_container, selected_stat, player);

    free(title_text);

    return EXIT_SUCCESS;
}

int display_stat_options_cli(game_window_t * game_window, rect_t container, stat_t selected_stat, player_t * player) {
    if (!player) {
        return EXIT_FAILURE;
    }

    rect_t * stat_rectangles = get_rectangle_layout(STAT_COUNT, &container, VERTICAL, (int)container.h / 20);

    for (stat_t i = 0; i < STAT_COUNT; i++) {
        unsigned int current_stat_amount = 0;
        switch(i) {
            case STAT_HP:
                current_stat_amount = player->hp_max;
                break;
            case STAT_MANA:
                current_stat_amount = player->mana_max;
                break;
            case STAT_DEFENCE:
                current_stat_amount = player->base_defense;
                break;
            case STAT_ATTACK:
                current_stat_amount = player->base_attack;
                break;
            case STAT_ACTION_POINTS:
                current_stat_amount = player->max_action_points;
                break;
            default:
                current_stat_amount = 0;
        }

        display_stat_option_cli(
                game_window,
                current_stat_amount,
                stat_display_name[i],
                stat_display_ascii_path[i],
                stat_rectangles[i],
                i == selected_stat
        );
    }

    return EXIT_SUCCESS;
}

int display_stat_option_cli(game_window_t * game_window, unsigned int amount, char * display_name, char * ascii_path, rect_t container, bool is_selected) {
    char * text = calloc(100, sizeof(char));
    snprintf(text, 100, "%s (%u)", display_name, amount);

    int x_padding = (int)container.w / 10;
    int y_padding = (int)container.h / 10;

    rect_t content_container = container;
    content_container.x += x_padding;
    content_container.y += y_padding;
    content_container.w -= 2 * x_padding;
    content_container.h -= 2 * y_padding;

    // display highlight if selected option
    if (is_selected) {
        cli_draw_fill_rect(game_window->matrix, container, (cli_char_t){'>', GREEN});
    }

    int image_size = content_container.w > content_container.h ? content_container.h : content_container.w / 5;
    int image_right_margin = image_size / 3;
    rect_t image_container = content_container;
    image_container.w = image_size;
    image_container.h = image_size;

    print_ascii_art_in_rectangle(game_window->matrix, ascii_path, image_container, ALIGN_CENTER, ALIGN_CENTER);

    rect_t text_container = content_container;
    text_container.x += image_container.w + image_right_margin;
    text_container.w -= image_container.w - image_right_margin;
    cli_print_text_in_rectangle(game_window->matrix, text_container, text, BLACK, ALIGN_START, ALIGN_CENTER, SMALL_TEXT);

    free(text);

    return EXIT_SUCCESS;
}
