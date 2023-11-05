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

    cli_render_clear(game_window->matrix, (cli_char_t){' ', WHITE});

    rect_t confirm_rect = {window_width / 4, window_height / 4, window_width / 2, window_height / 2};

    int nb_details = 7;
    rect_t * details_rects = get_rectangle_layout(nb_details, &confirm_rect, VERTICAL);

    rect_t go_back_rect = {1, 0, window_width / 2 - 1, 1};
    cli_print_text_in_rectangle(game_window->matrix, go_back_rect, "Go back", BLACK, ALIGN_START, ALIGN_START,
                                SMALL_TEXT);
    // TODO : display go back ascii art

    rect_t gold_rect = {window_width - (go_back_rect.x + go_back_rect.w), 0, window_width / 2, 1};
    cli_print_text_in_rectangle(game_window->matrix, gold_rect, player_gold_to_string(player), BLACK, ALIGN_END, ALIGN_START,
                                SMALL_TEXT);
    // TODO : display gold ascii art


    rect_t items_container = {(window_width - (3 * unit + 4 * unit_padding)) / 2, window_height - (3 * unit + 4 * unit_padding), 3 * unit + 4 * unit_padding, 3 * unit + 4 * unit_padding};
    rect_t * items_rects = get_rectangle_layout(ITEMS_PER_PAGE, &items_container, GRID);

    rect_t categories_container = {0, items_container.y - 1, window_width, 3};
    rect_t * categories_rect = get_rectangle_layout(3, &categories_container, HORIZONTAL);
    // leave space for cursor
    for (int i = 0; i < 3; i++) {
        categories_rect[i].w -= 1;
        categories_rect[i].x += 1;
    }
    cli_print_text_in_rectangle(game_window->matrix, categories_rect[0], "WEAPONS", BLACK, ALIGN_CENTER, ALIGN_START,
                                SMALL_TEXT);
    cli_print_text_in_rectangle(game_window->matrix, categories_rect[1], "ARMORS", BLACK, ALIGN_CENTER, ALIGN_START,
                                SMALL_TEXT);
    cli_print_text_in_rectangle(game_window->matrix, categories_rect[2], "POTIONS", BLACK, ALIGN_CENTER, ALIGN_START,
                                SMALL_TEXT);

    rect_t merchant_rect = {0, go_back_rect.y + go_back_rect.h, window_width / 4, window_height - (go_back_rect.h + items_container.h + categories_container.h)};
    print_ascii_art_in_rectangle(game_window->matrix, "../assets/items_mgmt/ascii/shop_owner.asciiart", merchant_rect, ALIGN_CENTER, ALIGN_CENTER);

    rect_t message_rect = {merchant_rect.x + merchant_rect.w, merchant_rect.y, window_width - merchant_rect.w, merchant_rect.h};
    cli_print_text_in_rectangle(game_window->matrix, message_rect, "Hello there", BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);


    switch (active_section) {
        case CATEGORIES: {
            rect_t aimed_container;
            if (active_category == GO_BACK) {
                aimed_container = go_back_rect;
            } else {
                aimed_container = categories_rect[active_category - 1];
            }
            if (display_cursor_cli(game_window->matrix, aimed_container) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            break;
        }

        case ITEMS:
            if (display_shop_items_cli(game_window->matrix, items_rects, active_category, active_item) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            if (display_cursor_cli(game_window->matrix, items_rects[active_item % ITEMS_PER_PAGE]) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            break;

        case CONFIRM:
            if (display_item_confirm_cli(game_window->matrix, &confirm_rect, details_rects, active_category,
                                         active_item)) {
                return EXIT_FAILURE;
            }
            if (display_cursor_cli(game_window->matrix,
                                   (active_confirmation == true) ? details_rects[nb_details - 2] : details_rects[
                                           nb_details - 1])) {
                return EXIT_FAILURE;
            }
        case ACTIONS:
        default:
            break;
    }

    return EXIT_SUCCESS;
}

int display_shop_items_cli(cli_matrix_t * matrix, rect_t * items_rects, category_options_t active_category, unsigned int active_item) {
    int first_item_to_print = ((int) active_item / ITEMS_PER_PAGE) * ITEMS_PER_PAGE;
    switch (active_category) {
        case ARMORS:
            for (int i = 0; i < ITEMS_PER_PAGE; i++) {
                array_node_t *armors = get_armors();
                armor_t *armor_to_print = get_value_at_index(armors, first_item_to_print + i);
                if (!armor_to_print) {
                    break;
                }
                if (print_ascii_art_in_rectangle(matrix, armor_to_print->ascii_path, items_rects[i], ALIGN_START,
                                                 ALIGN_START) == EXIT_FAILURE) {
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
                if (print_ascii_art_in_rectangle(matrix, weapon_to_print->ascii_path, items_rects[i], ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE){
                    return EXIT_FAILURE;
                }
            }
            break;

        case HEALTH_POTIONS:
        case MANA_POTIONS:
            if (active_category == HEALTH_POTIONS) {
                // TODO : highlight when item is active
            } else if (active_category == MANA_POTIONS) {
                // TODO : highlight when item is active
            }
            if (print_ascii_art_in_rectangle(matrix, "../assets/items_mgmt/ascii/health_potions.asciiart", items_rects[0], ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE){
                return EXIT_FAILURE;
            }
            if (print_ascii_art_in_rectangle(matrix, "../assets/items_mgmt/ascii/mana_potions.asciiart", items_rects[0], ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE){
                return EXIT_FAILURE;
            }
            break;

        case GO_BACK:
        default:
            break;
    }

    return EXIT_SUCCESS;
}

int display_cursor_cli(cli_matrix_t *matrix, rect_t aimed_container) {
    if (cli_print_text_in_rectangle(matrix, (rect_t) {aimed_container.x - 1, aimed_container.y, 1, 1}, ">", RED, ALIGN_START, ALIGN_START, SMALL_TEXT) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int display_item_confirm_cli(cli_matrix_t *matrix, rect_t * container, rect_t * details_rects,
                             category_options_t active_category, unsigned int active_item) {
    if (cli_draw_fill_rect(matrix, *container, (cli_char_t) {' ', BLACK})) {
        return EXIT_FAILURE;
    }
    if (cli_draw_stroke_rect(matrix, *container, (cli_char_t) {'.', BLUE})) {
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
            cli_print_text_in_rectangle(matrix, details_rects[0], buffer, BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';

            strcat(buffer, "Rarity : ");
            strcat(buffer, custom_itoa(armor_to_print->rarity, num_buffer, 10));
            cli_print_text_in_rectangle(matrix, details_rects[1], buffer, BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';
            num_buffer[0] = '\0';

            strcat(buffer, "Defense value : ");
            strcat(buffer, custom_itoa(armor_to_print->amount, num_buffer, 10));
            cli_print_text_in_rectangle(matrix, details_rects[2], buffer, BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';
            num_buffer[0] = '\0';

            strcat(buffer, "Uses : ");
            strcat(buffer, custom_itoa(armor_to_print->uses, num_buffer, 10));
            cli_print_text_in_rectangle(matrix, details_rects[3], buffer, BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';
            num_buffer[0] = '\0';

            strcat(buffer, "Cost : ");
            strcat(buffer, custom_itoa(armor_to_print->cost, num_buffer, 10));
            cli_print_text_in_rectangle(matrix, details_rects[4], buffer, BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';
            num_buffer[0] = '\0';
            break;
        }

        case WEAPONS: {
            array_node_t * weapons = get_weapons();
            weapon_t * weapon_to_print = get_value_at_index(weapons, (int) active_item);

            strcat(buffer, "Name : ");
            strcat(buffer, weapon_to_print->name);
            cli_print_text_in_rectangle(matrix, details_rects[0], buffer, BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';

            strcat(buffer, "Rarity : ");
            strcat(buffer, custom_itoa(weapon_to_print->rarity, num_buffer, 10));
            cli_print_text_in_rectangle(matrix, details_rects[1], buffer, BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';

            strcat(buffer, "Attack range : ");
            strcat(buffer, custom_itoa(weapon_to_print->min_attack, num_buffer, 10));
            strcat(buffer, " - ");
            strcat(buffer, custom_itoa(weapon_to_print->max_attack, num_buffer, 10));
            cli_print_text_in_rectangle(matrix, details_rects[2], buffer, BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';

            strcat(buffer, "Uses : ");
            strcat(buffer, custom_itoa(weapon_to_print->uses, num_buffer, 10));
            cli_print_text_in_rectangle(matrix, details_rects[3], buffer, BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';

            strcat(buffer, "Cost : ");
            strcat(buffer, custom_itoa(weapon_to_print->cost, num_buffer, 10));
            cli_print_text_in_rectangle(matrix, details_rects[4], buffer, BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            buffer[0] = '\0';
            break;
        }

        case HEALTH_POTIONS:
            cli_print_text_in_rectangle(matrix, details_rects[0], "Name : HEALTH POTION", BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            cli_print_text_in_rectangle(matrix, details_rects[1], "Healing power : 20", BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            break;

        case MANA_POTIONS:
            cli_print_text_in_rectangle(matrix, details_rects[0], "Name : MANA POTION", BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            cli_print_text_in_rectangle(matrix, details_rects[1], "Mana power : 20", BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
            break;

        case GO_BACK:
        default:
            break;
    }

    cli_print_text_in_rectangle(matrix, details_rects[5], "YES", BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
    cli_print_text_in_rectangle(matrix, details_rects[6], "NO", BLACK, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);

    return EXIT_SUCCESS;
}