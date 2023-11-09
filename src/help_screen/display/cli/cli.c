#include "cli.h"
#include "../../../ui_utils/ui_utils.h"

int display_tips_cli(game_window_t * game_window, rect_t container);
int display_tip_cli(game_window_t * game_window, char * key, char * effect, rect_t container);

int display_help_cli(game_window_t * game_window) {
    cli_render_clear(game_window->matrix, (cli_char_t){' ', WHITE});

    int window_width = 0;
    int window_height = 0;
    cli_get_window_size(&window_width, &window_height);
    resize_cli_matrix(game_window->matrix, window_height, window_width, ' ', WHITE);


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

    cli_print_text_in_rectangle(
            game_window->matrix,
            title_container,
            "Help",
            game_window->cli_color_palette->text,
            ALIGN_CENTER,
            ALIGN_START,
            MEDIUM_TEXT
        );


    display_tips_cli(game_window, stats_container);

    return EXIT_SUCCESS;
}

int display_tips_cli(game_window_t * game_window, rect_t container) {

    rect_t * tips_rectangles = get_rectangle_layout(NB_TIPS, &container, VERTICAL, (int)container.h / 20);

    for (int i = 0; i < NB_TIPS; i++) {
        display_tip_cli(
                game_window,
                tips[i][0],
                tips[i][1],
                tips_rectangles[i]
        );
    }

    return EXIT_SUCCESS;
}

int display_tip_cli(game_window_t * game_window, char * key, char * effect, rect_t container) {

    int x_padding = (int)container.w / 10;
    int y_padding = (int)container.h / 10;

    rect_t content_container = container;
    content_container.x += x_padding;
    content_container.y += y_padding;
    content_container.w -= 2 * x_padding;
    content_container.h -= 2 * y_padding;

    int key_height = content_container.h;
    int key_width = content_container.w / 5;

    int key_right_margin = key_width / 3;
    rect_t key_container = content_container;
    key_container.w = key_width;
    key_container.h = key_height;

    cli_print_text_in_rectangle(
            game_window->matrix,
            key_container,
            key,
            game_window->cli_color_palette->text,
            ALIGN_START,
            ALIGN_CENTER,
            SMALL_TEXT
    );

    rect_t text_container = content_container;
    text_container.x += key_container.w + key_right_margin;
    text_container.w -= key_container.w - key_right_margin;
    cli_print_text_in_rectangle(
            game_window->matrix,
            text_container,
            effect,
            game_window->cli_color_palette->text,
            ALIGN_CENTER,
            ALIGN_CENTER,
            SMALL_TEXT
    );

    return EXIT_SUCCESS;
}
