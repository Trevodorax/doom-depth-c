#include "gui.h"
#include "../../../utils/utils.h"
#include "../../../ui_utils/sdl_utils/sdl_utils.h"
#include "../../utils/shop_utils.h"
#include "../../../ui_utils/ui_utils.h"
#include "../../../inventory_screen/display/display.h"
#include "../../../inventory_screen/display/gui/gui.h"

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

    if (display_gold_gui(game_window->renderer, player, rect_to_SDL_Rect(gold_icon_rect), rect_to_SDL_Rect(gold_rect))) {
        return EXIT_FAILURE;
    }
    if (display_shop_categories_gui(game_window->renderer, &categories_container, rect_to_SDL_Rect(go_back_icon_rect),
                                    rect_to_SDL_Rect(go_back_text_rect), active_category)) {
        return EXIT_FAILURE;
    }
    if (display_merchant_gui(game_window->renderer, rect_to_SDL_Rect(merchant_image_rect),
                             rect_to_SDL_Rect(dialog_rect), "Hello there")) {
        return EXIT_FAILURE;
    }
    if (active_section == ITEMS &&
        display_shop_items_gui(game_window->renderer, &items_container, active_category, active_item)) {
        return EXIT_FAILURE;
    }
    if (active_section == CONFIRM && display_item_confirm_gui(game_window->renderer, rect_to_SDL_Rect(window_rect),
                                                              rect_to_SDL_Rect(confirm_rect), active_confirmation,
                                                              active_category, active_item)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_go_back_gui(SDL_Renderer * renderer, SDL_Rect icon_container, SDL_Rect text_container) {
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

int display_gold_gui(SDL_Renderer * renderer, player_t * player, SDL_Rect icon_container, SDL_Rect text_container) {
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

int display_shop_categories_gui(SDL_Renderer * renderer, rect_t * categories_container,
                                SDL_Rect go_back_icon_container, SDL_Rect go_back_text_container,
                                category_options_t active_category) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};
    if (display_go_back_gui(renderer, go_back_icon_container, go_back_text_container)) {
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

int display_merchant_gui(SDL_Renderer * renderer, SDL_Rect merchant_container, SDL_Rect message_container,
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

int display_shop_items_gui(SDL_Renderer * renderer,
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

    display_scroll_indicator_gui(renderer, rect_to_SDL_Rect(*items_container), 12, ITEMS_PER_PAGE, quantity, first_item_to_print);

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
                if (draw_image_in_rectangle(renderer, rect_to_SDL_Rect(items[i]), armor_to_print->image_path, NORTH, true, ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE){
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

int display_item_confirm_gui(SDL_Renderer *renderer, SDL_Rect window_rect, SDL_Rect container,
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