#include "gui.h"
#include "../../../ui_utils/sdl_utils/sdl_utils.h"
#include "../../../ui_utils/ui_utils.h"

int display_stat_options_gui(game_window_t * game_window, rect_t container, player_t * player);
int display_stat_option_gui(game_window_t * game_window, unsigned int amount, char * display_name, char * image_path, rect_t container);

int display_stats_gui(game_window_t * game_window, player_t * player) {
    set_background_color(game_window->renderer, game_window->sdl_color_palette->background);

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

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

    print_text_in_rectangle(
            game_window->renderer,
            rect_to_SDL_Rect(title_container),
            "Player stats",
            game_window->sdl_color_palette->text,
            ALIGN_CENTER,
            ALIGN_START
        );

    display_stat_options_gui(game_window, stats_container, player);

    return EXIT_SUCCESS;
}

int display_stat_options_gui(game_window_t * game_window, rect_t container, player_t * player) {
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
            case STAT_DEFENSE:
                current_stat_amount = player->base_defense;
                break;
            case STAT_ATTACK:
                current_stat_amount = player->base_attack;
                break;
            case STAT_ACTION_POINTS:
                current_stat_amount = player->max_action_points;
                break;
            case STAT_GOLD:
                current_stat_amount = player->gold;
                break;
            default:
                current_stat_amount = 0;
        }

        display_stat_option_gui(
                game_window,
                current_stat_amount,
                stat_display_name[i],
                stat_display_image_path[i],
                stat_rectangles[i]
            );
    }

    return EXIT_SUCCESS;
}

int display_stat_option_gui(game_window_t * game_window, unsigned int amount, char * display_name, char * image_path, rect_t container) {
    char * text = calloc(100, sizeof(char));
    snprintf(text, 100, "%s (%u)", display_name, amount);

    int x_padding = (int)container.w / 10;
    int y_padding = (int)container.h / 10;

    rect_t content_container = container;
    content_container.x += x_padding;
    content_container.y += y_padding;
    content_container.w -= 2 * x_padding;
    content_container.h -= 2 * y_padding;

    int image_size = content_container.w > content_container.h ? content_container.h : content_container.w / 5;
    int image_right_margin = image_size / 3;
    rect_t image_container = content_container;
    image_container.w = image_size;
    image_container.h = image_size;

    draw_image_in_rectangle(game_window->renderer, rect_to_SDL_Rect(image_container), image_path, NORTH, true, ALIGN_CENTER, ALIGN_CENTER);

    rect_t text_container = content_container;
    text_container.x += image_container.w + image_right_margin;
    text_container.w -= image_container.w - image_right_margin;
    print_text_in_rectangle(
            game_window->renderer,
            rect_to_SDL_Rect(text_container),
            text,
            game_window->sdl_color_palette->text,
            ALIGN_START,
            ALIGN_CENTER
    );

    free(text);

    return EXIT_SUCCESS;
}
