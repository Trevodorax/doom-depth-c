#include "display.h"
#include "../../utils/utils.h"
#include "../../utils/items_management/types.h"
#include "../../ui_utils/sdl_utils/sdl_utils.h"
#include "../utils/shop_utils.h"
#include "../../inventory_screen/display/display.h"
#include "../../ui_utils/ui_utils.h"
#include "../../inventory_screen/display/gui/gui.h"
#include "../../ui_utils/cli_utils/ascii_art/ascii_art.h"
#include "../../logs/log.h"

#define ITEMS_PER_PAGE 9

int display_shop_gui(game_window_t * game_window, player_t * player,
                     section_options_t active_section, category_options_t active_category,
                     bool active_confirmation, unsigned short active_item);
int display_go_back(SDL_Renderer * renderer, SDL_Rect icon_container, SDL_Rect text_container);
int display_gold(SDL_Renderer * renderer, player_t * player, SDL_Rect icon_container, SDL_Rect text_container);
int display_shop_categories(SDL_Renderer * renderer, rect_t * categories_container,
                            SDL_Rect go_back_icon_container, SDL_Rect go_back_text_container,
                            category_options_t active_category);
int display_merchant(SDL_Renderer * renderer, SDL_Rect merchant_container, SDL_Rect message_container,
                     const char * message);
int display_shop_items(SDL_Renderer * renderer, rect_t * items_container,
                       category_options_t active_category, unsigned short active_item);
int display_item_confirm(SDL_Renderer * renderer, SDL_Rect window_rect, SDL_Rect container,
                         bool active_confirmation, category_options_t active_category,
                         unsigned int active_item);

int display_shop_cli(game_window_t *game_window, player_t *player,
                     section_options_t active_section, category_options_t active_category,
                     bool active_confirmation, unsigned short active_item);
int cli_display_shop_items(cli_matrix_t * matrix, rect_t * items_rects, category_options_t active_category, unsigned int active_item);
int cli_display_cursor(cli_matrix_t *matrix, rect_t aimed_container);
int cli_display_item_confirm(cli_matrix_t *matrix, rect_t * container, rect_t * details_rects,
                             category_options_t active_category, unsigned int active_item);

int display_shop(game_window_t * game_window, player_t * player,
                 section_options_t active_section, category_options_t active_category,
                 bool active_confirmation, unsigned short active_item) {
    switch (game_window->ui_type) {
        case GUI:
            return display_shop_gui(game_window, player, active_section, active_category, active_confirmation, active_item);
        case CLI:
            return display_shop_cli(game_window, player, active_section, active_category, active_confirmation, active_item);
    }
}

int display_shop_gui(game_window_t * game_window,
                     player_t * player,
                     section_options_t active_section,
                     category_options_t active_category,
                     bool active_confirmation,
                     unsigned short active_item) {
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    int unit = (min(window_width, window_height) == window_width) ? window_width / 4 : window_height / 5;
    int unit_padding = unit / 10;
    int icon_size = 2 * unit_padding;
    int font_size = icon_size;

    SDL_RenderClear(game_window->renderer);

    rect_t window_rect = {0, 0, window_width, window_height};

    rect_t confirm_rect = {
        window_width / 6,
        window_height / 6,
        2 * window_width / 3,
        2 * window_height / 3
    };

    // draw black background
    draw_fill_rect(rect_to_SDL_Rect(window_rect), (SDL_Color) {0, 0, 0, 0}, game_window->renderer);

    rect_t go_back_icon_rect = {
        unit_padding,
        unit_padding,
        icon_size,
        icon_size,
    };

    rect_t go_back_text_rect = {
            unit_padding + go_back_icon_rect.w + unit_padding,
            unit_padding,
            window_width / 2 - (go_back_icon_rect.x + go_back_icon_rect.w),
            go_back_icon_rect.h
    };

    rect_t gold_icon_rect = {
        window_width - unit_padding - icon_size,
        unit_padding,
        icon_size,
        icon_size
    };

    rect_t gold_rect = {
            window_width / 2,
            unit_padding,
            window_width / 2 - (go_back_icon_rect.x + go_back_icon_rect.w),
            gold_icon_rect.h
    };

    rect_t items_container = {
        0,
        window_height - 3 * unit + unit_padding,
        3 * unit - 2 * unit_padding,
        3 * unit - 2 * unit_padding
    };
    items_container.x = (window_width - items_container.w) / 2;

    rect_t categories_container = {
        unit_padding,
        items_container.y - unit_padding - font_size,
        window_width - 2 * unit_padding,
        font_size
    };

    rect_t merchant_image_rect = {
        unit_padding,
        go_back_icon_rect.y + go_back_icon_rect.h + unit_padding,
        window_width / 4,
        categories_container.y - (go_back_icon_rect.y + go_back_icon_rect.h + 2 * unit_padding)
    };

    rect_t dialog_rect = {
        merchant_image_rect.x + merchant_image_rect.w + unit_padding,
        merchant_image_rect.y,
        window_width - (merchant_image_rect.x + merchant_image_rect.w + 2 * unit_padding),
        merchant_image_rect.h
    };

    if (display_gold(game_window->renderer, player, rect_to_SDL_Rect(gold_icon_rect), rect_to_SDL_Rect(gold_rect))) {
        return EXIT_FAILURE;
    }
    if (display_shop_categories(game_window->renderer, &categories_container, rect_to_SDL_Rect(go_back_icon_rect), rect_to_SDL_Rect(go_back_text_rect), active_category)) {
        return EXIT_FAILURE;
    }
    if (display_merchant(game_window->renderer, rect_to_SDL_Rect(merchant_image_rect), rect_to_SDL_Rect(dialog_rect), "Hello there")) {
        return EXIT_FAILURE;
    }
    if (active_section == ITEMS && display_shop_items(game_window->renderer, &items_container,active_category, active_item)) {
        return EXIT_FAILURE;
    }
    if (active_section == CONFIRM && display_item_confirm(game_window->renderer, rect_to_SDL_Rect(window_rect), rect_to_SDL_Rect(confirm_rect), active_confirmation, active_category, active_item)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_go_back(SDL_Renderer * renderer, SDL_Rect icon_container, SDL_Rect text_container) {
    if (draw_image_in_rectangle(
            renderer,
            icon_container,
            "../assets/items_mgmt/image/go_back.png",
            NORTH, true, ALIGN_CENTER, ALIGN_END
    )) {
        return EXIT_FAILURE;
    }

    if (print_text_in_rectangle(renderer, text_container,
                                "Go back", (SDL_Color) {255, 255, 255, 255},
                                ALIGN_START, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_gold(SDL_Renderer * renderer, player_t * player, SDL_Rect icon_container, SDL_Rect text_container) {
    if (draw_image_in_rectangle(
            renderer,
            icon_container,
            "../assets/items_mgmt/image/gold.png",
            NORTH, true, ALIGN_CENTER, ALIGN_END)
    ) {
        return EXIT_FAILURE;
    }

    if (print_text_in_rectangle(renderer, text_container,
                                player_gold_to_string(player), (SDL_Color) {255, 255, 255, 255},
                                ALIGN_END, ALIGN_CENTER)
    ) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_shop_categories(SDL_Renderer * renderer, rect_t * categories_container,
                       SDL_Rect go_back_icon_container, SDL_Rect go_back_text_container,
                       category_options_t active_category) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};
    if (display_go_back(renderer, go_back_icon_container, go_back_text_container)) {
        return EXIT_FAILURE;
    }

    rect_t *categories_rect = get_rectangle_layout(3, categories_container, HORIZONTAL);
    if (print_text_in_rectangle(renderer, rect_to_SDL_Rect(categories_rect[0]), "WEAPONS", white, ALIGN_CENTER, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }
    if (print_text_in_rectangle(renderer, rect_to_SDL_Rect(categories_rect[1]), "ARMORS", white, ALIGN_CENTER, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }
    if (print_text_in_rectangle(renderer, rect_to_SDL_Rect(categories_rect[2]), "POTIONS", white, ALIGN_CENTER, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }

    switch (active_category) {
        case GO_BACK:
            if (draw_thick_rect(go_back_icon_container, 2, white, renderer)) {
                return EXIT_FAILURE;
            }
            break;

        case ARMORS:
            if (draw_thick_rect(
                    (SDL_Rect) {(int) categories_rect[1].x, (int) categories_rect[1].y + (int) categories_rect[1].h - 2,
                                (int) categories_rect[1].w, 2},
                    1, white, renderer)) {
                return EXIT_FAILURE;
            }
            break;

        case WEAPONS:
            if (draw_thick_rect(
                    (SDL_Rect) {(int) categories_rect[0].x, (int) categories_rect[0].y + (int) categories_rect[0].h - 2,
                                (int) categories_rect[0].w, 2},
                    1, white, renderer)) {
                return EXIT_FAILURE;
            }
            break;

        case HEALTH_POTIONS:
        case MANA_POTIONS:
            if (draw_thick_rect(
                    (SDL_Rect) {(int) categories_rect[2].x, (int) categories_rect[2].y + (int) categories_rect[2].h - 2,
                                (int) categories_rect[2].w, 2},
                    1, white, renderer)) {
                return EXIT_FAILURE;
            }
            break;
    }

    return EXIT_SUCCESS;
}

int display_merchant(SDL_Renderer * renderer, SDL_Rect merchant_container, SDL_Rect message_container,
                     const char *message) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};
    SDL_Color black = (SDL_Color) {0, 0, 0, 255};
    if (draw_image_in_rectangle(renderer, merchant_container, "../assets/items_mgmt/image/shop_owner.png", NORTH, true, ALIGN_CENTER, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }

    draw_fill_rect(message_container, white, renderer);
    if (print_text_in_rectangle(renderer, message_container, message, black, ALIGN_CENTER, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_shop_items(SDL_Renderer * renderer,
                       rect_t * items_container,
                       category_options_t active_category,
                       unsigned short active_item) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};

    int quantity = -1;
    switch (active_category) {
        case GO_BACK:
            quantity = -1;
            break;

        case WEAPONS: {
            array_node_t * weapons = get_weapons();
            quantity = get_count(weapons);
            break;
        }

        case ARMORS: {
            array_node_t * armors = get_armors();
            quantity = get_count(armors);
            break;
        }

        case HEALTH_POTIONS:
        case MANA_POTIONS:
            quantity = 2;
    }

    rect_t *items = get_rectangle_layout(ITEMS_PER_PAGE, items_container, GRID);

    int first_item_to_print = (active_item / ITEMS_PER_PAGE) * ITEMS_PER_PAGE;

    display_scroll_indicator_gui(renderer, rect_to_SDL_Rect(*items_container), font_size, ITEMS_PER_PAGE, quantity, first_item_to_print);

    switch (active_category) {
        case ARMORS:
            for (int i = 0; i < ITEMS_PER_PAGE; i++) {
                array_node_t * armors = get_armors();
                armor_t * armor_to_print = get_value_at_index(armors, first_item_to_print + i);
                if (!armor_to_print) {
                    break;
                }
                if (active_item % ITEMS_PER_PAGE == i) {
                    draw_thick_rect(rect_to_SDL_Rect(items[i]), 2, white, renderer);
                }
                if (draw_image_in_rectangle(renderer, rect_to_SDL_Rect(items[i]), armor_to_print->image_path, NORTH, true, ALIGN_CENTER, ALIGN_CENTER)){
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
                if (active_item % ITEMS_PER_PAGE == i) {
                    draw_thick_rect(rect_to_SDL_Rect(items[i]), 2, white, renderer);
                }
                if (draw_image_in_rectangle(renderer, rect_to_SDL_Rect(items[i]), weapon_to_print->image_path, NORTH, true, ALIGN_CENTER, ALIGN_CENTER)){
                    return EXIT_FAILURE;
                }
            }
            break;

        case HEALTH_POTIONS:
        case MANA_POTIONS:
            if (active_category == HEALTH_POTIONS) {
                draw_thick_rect(rect_to_SDL_Rect(items[0]), 2, white, renderer);
            } else if (active_category == MANA_POTIONS) {
                draw_thick_rect(rect_to_SDL_Rect(items[1]), 2, white, renderer);
            }
            if (draw_image_in_rectangle(renderer, rect_to_SDL_Rect(items[0]), "../assets/items_mgmt/image/health_potions.png", NORTH, true, ALIGN_CENTER, ALIGN_CENTER)){
                return EXIT_FAILURE;
            }
            if (draw_image_in_rectangle(renderer, rect_to_SDL_Rect(items[1]), "../assets/items_mgmt/image/mana_potions.png", NORTH, true, ALIGN_CENTER, ALIGN_CENTER)){
                return EXIT_FAILURE;
            }
            break;

        case GO_BACK:
        default:
            break;
    }

    return EXIT_SUCCESS;
}

int display_item_confirm(SDL_Renderer *renderer, SDL_Rect window_rect, SDL_Rect container,
                         bool active_confirmation, category_options_t active_category,
                         unsigned int active_item) {
    if (draw_image_in_rectangle(renderer, window_rect, "../assets/backgrounds/white_semi_transparent.png", NORTH, false, ALIGN_START, ALIGN_START)) {
        return EXIT_FAILURE;
    }
    if (draw_fill_rect(container, (SDL_Color) {0, 0, 0, 255}, renderer)) {
        return EXIT_FAILURE;
    }

    char *details = NULL;
    switch (active_category) {
        case ARMORS: {
            array_node_t * armors = get_armors();
            details = shop_armor_details_to_string(get_value_at_index(armors, (int) active_item));
            break;
        }

        case WEAPONS: {
            array_node_t * weapons = get_weapons();
            details = shop_weapon_details_to_string(get_value_at_index(weapons, (int) active_item));
            break;
        }

        case HEALTH_POTIONS:
            details = shop_health_potions_details_to_string();
            break;

        case MANA_POTIONS:
            details = shop_mana_potions_details_to_string();
            break;

        case GO_BACK:
        default:
            break;
    }

    char * confirmation_string = active_confirmation ? ">YES\n\n   NO" : "   YES\n\n>NO";
    details = realloc(details, sizeof(char) * (strlen(details) + strlen(confirmation_string) + 1));
    strcat(details, confirmation_string);

    print_text_in_rectangle(renderer, container, details, (SDL_Color) {255, 255, 255, 255}, ALIGN_CENTER, ALIGN_CENTER);

    return EXIT_SUCCESS;
}

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
            if (cli_display_cursor(game_window->matrix, aimed_container) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            break;
        }

        case ITEMS:
            if (cli_display_shop_items(game_window->matrix, items_rects, active_category, active_item) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            if (cli_display_cursor(game_window->matrix, items_rects[active_item % ITEMS_PER_PAGE]) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            break;

        case CONFIRM:
            if (cli_display_item_confirm(game_window->matrix, &confirm_rect, details_rects, active_category, active_item)) {
                return EXIT_FAILURE;
            }
            if (cli_display_cursor(game_window->matrix, (active_confirmation == true) ? details_rects[nb_details - 2] : details_rects[nb_details - 1])) {
                return EXIT_FAILURE;
            }
        case ACTIONS:
        default:
            break;
    }

    return EXIT_SUCCESS;
}

int cli_display_shop_items(cli_matrix_t * matrix, rect_t * items_rects, category_options_t active_category, unsigned int active_item) {
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

int cli_display_cursor(cli_matrix_t *matrix, rect_t aimed_container) {
    if (cli_print_text_in_rectangle(matrix, (rect_t) {aimed_container.x - 1, aimed_container.y, 1, 1}, ">", RED, ALIGN_START, ALIGN_START, SMALL_TEXT) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int cli_display_item_confirm(cli_matrix_t *matrix, rect_t * container, rect_t * details_rects,
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