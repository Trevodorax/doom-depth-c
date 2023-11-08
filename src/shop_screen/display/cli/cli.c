#include "cli.h"
#include "../../../ui_utils/ui_utils.h"
#include "../../utils/shop_utils.h"
#include "../../../ui_utils/cli_utils/ascii_art/ascii_art.h"
#include "../../../utils/utils.h"

int display_shop_cli(game_window_t * game_window,
                     player_t * player,
                     section_options_t active_section,
                     category_options_t active_category,
                     bool active_confirmation,
                     unsigned short active_item){
    int window_width = 0;
    int window_height = 0;
    cli_get_window_size(&window_width, &window_height);

    int unit = (min(window_width, window_height) == window_width) ? window_width / 4 : window_height / 5;
    int unit_padding = (unit / 10 < 1) ? 1 : unit / 10;

    cli_render_clear(game_window->matrix, (cli_char_t){' ', game_window->cli_color_palette->text});

    rect_t confirm_rect = {window_width / 4, window_height / 4, window_width / 2, window_height / 2};

    int nb_details = 7;
    rect_t * details_rects = get_rectangle_layout(nb_details, &confirm_rect, VERTICAL, 2);

    rect_t go_back_rect = {1, 0, window_width / 2 - 1, 1};
    cli_print_text_in_rectangle(game_window->matrix, go_back_rect, "<-- Go back",game_window->cli_color_palette->text,
                                ALIGN_START, ALIGN_START, SMALL_TEXT);

    rect_t gold_rect = {window_width - (go_back_rect.x + go_back_rect.w), 0, window_width / 2, 1};
    char * buffer = malloc(sizeof(char) * 20);
    buffer = strdup(player_gold_to_string(player));
    strcat(buffer, " $");
    cli_print_text_in_rectangle(game_window->matrix, gold_rect, buffer, game_window->cli_color_palette->text,
                                ALIGN_END, ALIGN_START, SMALL_TEXT);
    free(buffer);

    rect_t items_container = {
            (window_width - (3 * unit + 4 * unit_padding)) / 2, window_height - (3 * unit + 4 * unit_padding),
            3 * unit + 4 * unit_padding, 3 * unit + 4 * unit_padding
    };
    rect_t * items_rects = get_rectangle_layout(ITEMS_PER_PAGE, &items_container, GRID, 2);

    rect_t categories_container = {0, items_container.y - 1, window_width, 3};
    rect_t * categories_rect = get_rectangle_layout(3, &categories_container, HORIZONTAL, 2);
    // leave space for cursor
    for (int i = 0; i < 3; i++) {
        categories_rect[i].w -= 1;
        categories_rect[i].x += 1;
    }
    cli_print_text_in_rectangle(game_window->matrix, categories_rect[0], "WEAPONS", game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_START, SMALL_TEXT);
    cli_print_text_in_rectangle(game_window->matrix, categories_rect[1], "ARMORS", game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_START, SMALL_TEXT);
    cli_print_text_in_rectangle(game_window->matrix, categories_rect[2], "POTIONS", game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_START, SMALL_TEXT);

    rect_t merchant_rect = {
            0, go_back_rect.y + go_back_rect.h,
            window_width / 4, window_height - (go_back_rect.h + items_container.h + categories_container.h)
    };
    print_ascii_art_in_rectangle(game_window->matrix, "../assets/items_mgmt/ascii/shop_owner.asciiart",
                                 merchant_rect, ALIGN_CENTER, ALIGN_CENTER);

    rect_t message_rect = {
            merchant_rect.x + merchant_rect.w, merchant_rect.y,
            window_width - merchant_rect.w, merchant_rect.h
    };
    cli_print_text_in_rectangle(game_window->matrix, message_rect, "Hello there",game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_START, SMALL_TEXT);


    switch (active_section) {
        case CATEGORIES: {
            rect_t aimed_container;
            if (active_category == GO_BACK) {
                aimed_container = go_back_rect;
            } else {
                aimed_container = categories_rect[active_category - 1];
            }
            if (display_cursor_cli(game_window, aimed_container) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            break;
        }

        case ITEMS:
            if (display_shop_items_cli(game_window, items_rects, active_category, active_item) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            if ((active_category == ARMORS || active_category == WEAPONS) &&
                display_cursor_cli(game_window, items_rects[active_item % ITEMS_PER_PAGE]) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            break;

        case CONFIRM:
            if (display_item_confirm_cli(game_window, &confirm_rect, details_rects, active_category, active_item)) {
                return EXIT_FAILURE;
            }
            if (display_cursor_cli(game_window, (active_confirmation == true) ? details_rects[nb_details - 2] : details_rects[nb_details - 1])) {
                return EXIT_FAILURE;
            }
        case ACTIONS:
        default:
            break;
    }

    return EXIT_SUCCESS;
}

int display_shop_items_cli(game_window_t * game_window, rect_t * items_rects, category_options_t active_category, unsigned int active_item) {
    int first_item_to_print = ((int) active_item / ITEMS_PER_PAGE) * ITEMS_PER_PAGE;
    switch (active_category) {
        case ARMORS:
            for (int i = 0; i < ITEMS_PER_PAGE; i++) {
                array_node_t *armors = get_armors();
                armor_t *armor_to_print = get_value_at_index(armors, first_item_to_print + i);
                if (!armor_to_print) {
                    break;
                }
                if (print_ascii_art_in_rectangle(game_window->matrix, armor_to_print->ascii_path,
                                                 items_rects[i], ALIGN_START, ALIGN_START) == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
            }
            break;

        case WEAPONS:
            for (int i = 0; i < ITEMS_PER_PAGE; i++) {
                array_node_t * weapons = get_weapons();
                weapon_t * weapon_to_print = get_value_at_index(weapons, first_item_to_print + i);
                if (!weapon_to_print) {
                    break;
                }
                if (print_ascii_art_in_rectangle(game_window->matrix, weapon_to_print->ascii_path,
                                                 items_rects[i], ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE){
                    return EXIT_FAILURE;
                }
            }
            break;

        case HEALTH_POTIONS:
            items_rects[0].x += 1;
            items_rects[0].w -= 1;
            items_rects[1].x += 1;
            items_rects[1].w -= 1;

            if(display_cursor_cli(game_window, items_rects[0])) {
                return EXIT_FAILURE;
            }
            if (print_ascii_art_in_rectangle(game_window->matrix, "../assets/items_mgmt/ascii/health_potions.asciiart",
                                             items_rects[0], ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE){
                return EXIT_FAILURE;
            }
            if (print_ascii_art_in_rectangle(game_window->matrix, "../assets/items_mgmt/ascii/mana_potions.asciiart",
                                             items_rects[1], ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE){
                return EXIT_FAILURE;
            }
            break;

        case MANA_POTIONS:
            items_rects[0].x += 1;
            items_rects[0].w -= 1;
            items_rects[1].x += 1;
            items_rects[1].w -= 1;

            if(display_cursor_cli(game_window, items_rects[1])) {
                return EXIT_FAILURE;
            }
            if (print_ascii_art_in_rectangle(game_window->matrix, "../assets/items_mgmt/ascii/health_potions.asciiart",
                                             items_rects[0], ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE){
                return EXIT_FAILURE;
            }
            if (print_ascii_art_in_rectangle(game_window->matrix, "../assets/items_mgmt/ascii/mana_potions.asciiart",
                                             items_rects[1], ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE){
                return EXIT_FAILURE;
            }
            break;

        case GO_BACK:
        default:
            break;
    }

    return EXIT_SUCCESS;
}

int display_cursor_cli(game_window_t * game_window, rect_t aimed_container) {
    if (cli_print_text_in_rectangle(game_window->matrix, (rect_t) {aimed_container.x - 1, aimed_container.y, 1, 1},
                                    ">", game_window->cli_color_palette->highlight,
                                    ALIGN_START, ALIGN_START, SMALL_TEXT) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int display_item_confirm_cli(game_window_t * game_window, rect_t * container, rect_t * details_rects,
                             category_options_t active_category, unsigned int active_item) {
    if (cli_draw_fill_rect(game_window->matrix, *container, (cli_char_t) {' ', game_window->cli_color_palette->text})) {
        return EXIT_FAILURE;
    }
    if (cli_draw_stroke_rect(game_window->matrix, *container, (cli_char_t) {'.', game_window->cli_color_palette->blue})) {
        return EXIT_FAILURE;
    }

    char * buffer = malloc(sizeof(char) * 100);
    buffer[0] = '\0';
    char * num_buffer = malloc(sizeof(char) * 4);
    num_buffer[0] = '\0';
    switch (active_category) {
        case ARMORS: {
            array_node_t * armors = get_armors();
            armor_t * armor_to_print = get_value_at_index(armors, (int) active_item);

            strcat(buffer, "Name : ");
            strcat(buffer, armor_to_print->name);
            cli_print_text_in_rectangle(game_window->matrix, details_rects[0], buffer, game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';

            strcat(buffer, "Rarity : ");
            strcat(buffer, custom_itoa(armor_to_print->rarity, num_buffer, 10));
            cli_print_text_in_rectangle(game_window->matrix, details_rects[1], buffer, game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';
            num_buffer[0] = '\0';

            strcat(buffer, "Defense value : ");
            strcat(buffer, custom_itoa(armor_to_print->amount, num_buffer, 10));
            cli_print_text_in_rectangle(game_window->matrix, details_rects[2], buffer, game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';
            num_buffer[0] = '\0';

            strcat(buffer, "Uses : ");
            strcat(buffer, custom_itoa(armor_to_print->uses, num_buffer, 10));
            cli_print_text_in_rectangle(game_window->matrix, details_rects[3], buffer, game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';
            num_buffer[0] = '\0';

            strcat(buffer, "Cost : ");
            strcat(buffer, custom_itoa(armor_to_print->cost, num_buffer, 10));
            cli_print_text_in_rectangle(game_window->matrix, details_rects[4], buffer, game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';
            num_buffer[0] = '\0';
            break;
        }

        case WEAPONS: {
            array_node_t * weapons = get_weapons();
            weapon_t * weapon_to_print = get_value_at_index(weapons, (int) active_item);

            strcat(buffer, "Name : ");
            strcat(buffer, weapon_to_print->name);
            cli_print_text_in_rectangle(game_window->matrix, details_rects[0], buffer, game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';

            strcat(buffer, "Rarity : ");
            strcat(buffer, custom_itoa(weapon_to_print->rarity, num_buffer, 10));
            cli_print_text_in_rectangle(game_window->matrix, details_rects[1], buffer, game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';

            strcat(buffer, "Attack range : ");
            strcat(buffer, custom_itoa(weapon_to_print->min_attack, num_buffer, 10));
            strcat(buffer, " - ");
            strcat(buffer, custom_itoa(weapon_to_print->max_attack, num_buffer, 10));
            cli_print_text_in_rectangle(game_window->matrix, details_rects[2], buffer, game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';

            strcat(buffer, "Uses : ");
            strcat(buffer, custom_itoa(weapon_to_print->uses, num_buffer, 10));
            cli_print_text_in_rectangle(game_window->matrix, details_rects[3], buffer, game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';

            strcat(buffer, "Cost : ");
            strcat(buffer, custom_itoa(weapon_to_print->cost, num_buffer, 10));
            cli_print_text_in_rectangle(game_window->matrix, details_rects[4], buffer, game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';
            break;
        }

        case HEALTH_POTIONS:
            cli_print_text_in_rectangle(game_window->matrix, details_rects[0], "Name : HEALTH POTION", game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            cli_print_text_in_rectangle(game_window->matrix, details_rects[1], "Healing power : 20", game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            break;

        case MANA_POTIONS:
            cli_print_text_in_rectangle(game_window->matrix, details_rects[0], "Name : MANA POTION", game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            cli_print_text_in_rectangle(game_window->matrix, details_rects[1], "Mana power : 20", game_window->cli_color_palette->text,
                                        ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            break;

        case GO_BACK:
        default:
            break;
    }

    cli_print_text_in_rectangle(game_window->matrix, details_rects[5], "YES", game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
    cli_print_text_in_rectangle(game_window->matrix, details_rects[6], "NO", game_window->cli_color_palette->text,
                                ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);

    return EXIT_SUCCESS;
}