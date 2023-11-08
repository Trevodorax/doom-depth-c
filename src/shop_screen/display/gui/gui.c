#include "gui.h"
#include "../../../utils/utils.h"
#include "../../../ui_utils/sdl_utils/sdl_utils.h"
#include "../../utils/shop_utils.h"
#include "../../../ui_utils/ui_utils.h"
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
    draw_fill_rect(rect_to_SDL_Rect(window_rect), game_window->sdl_color_palette->background, game_window->renderer);

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

    if (display_gold_gui(game_window, player, rect_to_SDL_Rect(gold_icon_rect), rect_to_SDL_Rect(gold_rect))) {
        return EXIT_FAILURE;
    }
    if (display_shop_categories_gui(game_window, &categories_container, rect_to_SDL_Rect(go_back_icon_rect),
                                    rect_to_SDL_Rect(go_back_text_rect), active_category)) {
        return EXIT_FAILURE;
    }
    if (display_merchant_gui(game_window, rect_to_SDL_Rect(merchant_image_rect),
                             rect_to_SDL_Rect(dialog_rect), "Hello there")) {
        return EXIT_FAILURE;
    }
    if (active_section == ITEMS &&
        display_shop_items_gui(game_window, &items_container, active_category, active_item)) {
        return EXIT_FAILURE;
    }
    if (active_section == CONFIRM && display_item_confirm_gui(game_window, rect_to_SDL_Rect(window_rect),
                                                              &confirm_rect, active_confirmation,
                                                              active_category, active_item)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_go_back_gui(game_window_t * game_window, SDL_Rect icon_container, SDL_Rect text_container) {
    if (draw_image_in_rectangle(
            game_window->renderer,
            icon_container,
            "../assets/items_mgmt/image/go_back.png",
            NORTH, true, ALIGN_CENTER, ALIGN_END
    )) {
        return EXIT_FAILURE;
    }

    if (print_text_in_rectangle(game_window->renderer, text_container,
                                "Go back", game_window->sdl_color_palette->text,
                                ALIGN_START, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_gold_gui(game_window_t * game_window, player_t * player, SDL_Rect icon_container, SDL_Rect text_container) {
    if (draw_image_in_rectangle(
            game_window->renderer,
            icon_container,
            "../assets/items_mgmt/image/gold.png",
            NORTH, true, ALIGN_CENTER, ALIGN_END)
            ) {
        return EXIT_FAILURE;
    }

    if (print_text_in_rectangle(game_window->renderer, text_container,
                                player_gold_to_string(player), game_window->sdl_color_palette->text,
                                ALIGN_END, ALIGN_CENTER)
            ) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_shop_categories_gui(game_window_t * game_window, rect_t * categories_container,
                                SDL_Rect go_back_icon_container, SDL_Rect go_back_text_container,
                                category_options_t active_category) {
    if (display_go_back_gui(game_window, go_back_icon_container, go_back_text_container)) {
        return EXIT_FAILURE;
    }

    rect_t *categories_rect = get_rectangle_layout(3, categories_container, HORIZONTAL, 2);
    if (print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(categories_rect[0]), "WEAPONS",
                                game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }
    if (print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(categories_rect[1]), "ARMORS",
                                game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }
    if (print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(categories_rect[2]), "POTIONS",
                                game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }

    switch (active_category) {
        case GO_BACK:
            if (draw_thick_rect(go_back_icon_container, 2, game_window->sdl_color_palette->text, game_window->renderer)) {
                return EXIT_FAILURE;
            }
            break;

        case ARMORS:
            if (draw_thick_rect(
                    (SDL_Rect) {(int) categories_rect[1].x, (int) categories_rect[1].y + (int) categories_rect[1].h - 2,
                                (int) categories_rect[1].w, 2},
                    1, game_window->sdl_color_palette->text, game_window->renderer)) {
                return EXIT_FAILURE;
            }
            break;

        case WEAPONS:
            if (draw_thick_rect(
                    (SDL_Rect) {(int) categories_rect[0].x, (int) categories_rect[0].y + (int) categories_rect[0].h - 2,
                                (int) categories_rect[0].w, 2},
                    1, game_window->sdl_color_palette->text, game_window->renderer)) {
                return EXIT_FAILURE;
            }
            break;

        case HEALTH_POTIONS:
        case MANA_POTIONS:
            if (draw_thick_rect(
                    (SDL_Rect) {(int) categories_rect[2].x, (int) categories_rect[2].y + (int) categories_rect[2].h - 2,
                                (int) categories_rect[2].w, 2},
                    1, game_window->sdl_color_palette->text, game_window->renderer)) {
                return EXIT_FAILURE;
            }
            break;
    }

    return EXIT_SUCCESS;
}

int display_merchant_gui(game_window_t * game_window, SDL_Rect merchant_container, SDL_Rect message_container,
                         const char *message) {
    if (draw_image_in_rectangle(game_window->renderer, merchant_container, "../assets/items_mgmt/image/shop_owner.png",
                                NORTH, true, ALIGN_CENTER, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }

    if (print_text_in_rectangle(game_window->renderer, message_container, message,
                                game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int display_shop_items_gui(game_window_t * game_window,
                           rect_t * items_container,
                           category_options_t active_category,
                           unsigned short active_item) {
    int quantity = -1;
    switch (active_category) {
        case GO_BACK:
            quantity = -1;
            break;

        case WEAPONS: {
            list_t * weapons = get_weapons();
            quantity = weapons->size;
            break;
        }

        case ARMORS: {
            list_t * armors = get_armors();
            quantity = armors->size;
            break;
        }

        case HEALTH_POTIONS:
        case MANA_POTIONS:
            quantity = 2;
    }

    rect_t *items = get_rectangle_layout(ITEMS_PER_PAGE, items_container, GRID, 2);

    int first_item_to_print = (active_item / ITEMS_PER_PAGE) * ITEMS_PER_PAGE;

    display_scroll_indicator_gui(game_window, rect_to_SDL_Rect(*items_container), 12, ITEMS_PER_PAGE, quantity, first_item_to_print);

    switch (active_category) {
        case ARMORS:
            for (int i = 0; i < ITEMS_PER_PAGE; i++) {
                list_t * armors = get_armors();
                armor_t * armor_to_print = get_value_at_index(armors, first_item_to_print + i);
                if (!armor_to_print) {
                    break;
                }
                if (active_item % ITEMS_PER_PAGE == i) {
                    draw_thick_rect(rect_to_SDL_Rect(items[i]), 2, game_window->sdl_color_palette->text, game_window->renderer);
                }
                if (draw_image_in_rectangle(game_window->renderer, rect_to_SDL_Rect(items[i]), armor_to_print->image_path,
                                            NORTH, true, ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE){
                    return EXIT_FAILURE;
                }
            }
            break;

        case WEAPONS:
            for (int i = 0; i < ITEMS_PER_PAGE; i++) {
                list_t * weapons = get_weapons();
                weapon_t * weapon_to_print = get_value_at_index(weapons, first_item_to_print + i);
                if (!weapon_to_print) {
                    break;
                }
                if (active_item % ITEMS_PER_PAGE == i) {
                    draw_thick_rect(rect_to_SDL_Rect(items[i]), 2, game_window->sdl_color_palette->text, game_window->renderer);
                }
                if (draw_image_in_rectangle(game_window->renderer, rect_to_SDL_Rect(items[i]), weapon_to_print->image_path,
                                            NORTH, true, ALIGN_CENTER, ALIGN_CENTER)){
                    return EXIT_FAILURE;
                }
            }
            break;

        case HEALTH_POTIONS:
        case MANA_POTIONS:
            if (active_category == HEALTH_POTIONS) {
                draw_thick_rect(rect_to_SDL_Rect(items[0]), 2, game_window->sdl_color_palette->text, game_window->renderer);
            } else if (active_category == MANA_POTIONS) {
                draw_thick_rect(rect_to_SDL_Rect(items[1]), 2, game_window->sdl_color_palette->text, game_window->renderer);
            }
            if (draw_image_in_rectangle(game_window->renderer, rect_to_SDL_Rect(items[0]), "../assets/items_mgmt/image/health_potions.png",
                                        NORTH, true, ALIGN_CENTER, ALIGN_CENTER)){
                return EXIT_FAILURE;
            }
            if (draw_image_in_rectangle(game_window->renderer, rect_to_SDL_Rect(items[1]), "../assets/items_mgmt/image/mana_potions.png",
                                        NORTH, true, ALIGN_CENTER, ALIGN_CENTER)){
                return EXIT_FAILURE;
            }
            break;

        case GO_BACK:
        default:
            break;
    }

    return EXIT_SUCCESS;
}

int display_item_confirm_gui(game_window_t * game_window, SDL_Rect window_rect, rect_t * container,
                             bool active_confirmation, category_options_t active_category,
                             unsigned int active_item) {
    unsigned long container_new_height = container->h * .8;

    rect_t details_container  = {
            container->x, container->y,
            container->w, container_new_height
    };
    rect_t confirmation_container = {
            container->x + container->w / 4, container->y + container_new_height,
            container->w / 2, container->h - container_new_height
    };
    rect_t * confirmation_rects = get_rectangle_layout(2, &confirmation_container, VERTICAL, 2);

    if (draw_image_in_rectangle(game_window->renderer, window_rect, "../assets/backgrounds/white_semi_transparent.png",
                                NORTH, false, ALIGN_START, ALIGN_START)) {
        return EXIT_FAILURE;
    }
    if (draw_fill_rect(rect_to_SDL_Rect(*container), game_window->sdl_color_palette->background, game_window->renderer)) {
        return EXIT_FAILURE;
    }

    char *details = NULL;
    switch (active_category) {
        case ARMORS: {
            list_t * armors = get_armors();
            details = shop_armor_details_to_string(get_value_at_index(armors, (int) active_item));
            break;
        }

        case WEAPONS: {
            list_t * weapons = get_weapons();
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

    print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(details_container), details,
                            game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER);
    print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(confirmation_rects[0]), "YES",
                            game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER);
    print_text_in_rectangle(game_window->renderer, rect_to_SDL_Rect(confirmation_rects[1]), "NO",
                            game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER);

    display_cursor_gui(game_window, (active_confirmation == true) ? &confirmation_rects[0] : &confirmation_rects[1]);

    return EXIT_SUCCESS;
}

int display_cursor_gui(game_window_t * game_window, rect_t * aimed_container) {
    SDL_Texture * cursor_texture = get_string_texture(game_window->renderer, ">",
                                  "../assets/PixelifySans-Regular.ttf", 12, game_window->sdl_color_palette->highlight);
    if (!cursor_texture) {
        return EXIT_FAILURE;
    }

    int cursor_width = 0;
    int cursor_height = 0;
    SDL_QueryTexture(cursor_texture, NULL, NULL, &cursor_width, &cursor_height);

    rect_t cursor_rect = {
            (aimed_container->x - 2 * cursor_width > 0) ? aimed_container->x - 2 * cursor_width : 0,
            aimed_container->y + (aimed_container->h - cursor_height) / 2,
            cursor_width, cursor_height
    };

    aimed_container->x += cursor_width;
    aimed_container->w -= cursor_width;

    SDL_Rect cursor_sdl_rect = rect_to_SDL_Rect(cursor_rect);

    SDL_RenderCopy(game_window->renderer, cursor_texture, NULL, &cursor_sdl_rect);

    return EXIT_SUCCESS;
}