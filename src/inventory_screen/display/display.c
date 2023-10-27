#include "display.h"
#include "../utils/inventory_utils.h"
#include "../../utils/utils.h"

#define ACTIONS_COUNT 2

int display_inventory(game_window_t *game_window,
                      inventory_t *inventory,
                      section_options_t active_section,
                      category_options_t active_category,
                      action_options_t active_action,
                      unsigned short active_item) {
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    SDL_RenderClear(game_window->renderer);

    draw_fill_rect(
            (SDL_Rect){0, 0, window_width, window_height},
            (SDL_Color){0, 0, 0, 255},
            game_window->renderer
    );

    int unit = (min(window_width, window_height) == window_height) ? window_height / 3 : window_width / 4;
    int unit_padding = unit / 10;
    int category_size = unit / 5;

    display_categories(game_window->renderer, category_size, unit_padding, active_section, active_category);
    if (active_section == CATEGORIES) {
        SDL_RenderPresent(game_window->renderer);
        return EXIT_SUCCESS;
    }

    SDL_Rect items_container = (SDL_Rect) {
            category_size + 2 * unit_padding,
            unit_padding,
            3 * unit - 2 * unit_padding,
            3 * unit - 2 * unit_padding
    };

    SDL_Rect item_details_container = (SDL_Rect) {
            category_size + 3 * unit + 2 * unit_padding,
            unit_padding,
            window_width - (items_container.x + + 3 * unit),
            2 * unit +unit_padding
    };
    SDL_Rect actions_container = (SDL_Rect) {
            category_size + 3 * unit + 2 * unit_padding,
            unit_padding + 2 * unit,
            window_width - (category_size + 3 * unit) - unit_padding,
            unit
    };

    switch (active_category) {
        case WEAPONS:
            if (!inventory->nb_weapons) {
                display_nothing_to_see(game_window->renderer, &items_container);
            } else {
                display_items(game_window->renderer, &items_container, &item_details_container, WEAPON, inventory, unit_padding - 2, active_item, active_section);
            }
            break;

        case ARMORS:
            if (!inventory->nb_armors) {
                display_nothing_to_see(game_window->renderer, &items_container);
            } else {
                display_items(game_window->renderer, &items_container, &item_details_container, ARMOR, inventory, unit_padding - 2, active_item, active_section);
            }
            break;

        case HEALTH_POTIONS:
            if (!inventory->nb_health_potions) {
                display_nothing_to_see(game_window->renderer, &items_container);
            } else {
                display_potions(game_window->renderer, HEALTH, &item_details_container, inventory->nb_health_potions, unit_padding - 2);
            }
            break;

        case MANA_POTIONS:
            if (inventory->nb_mana_potions == 0) {
                display_nothing_to_see(game_window->renderer, &items_container);
            } else {
                display_potions(game_window->renderer, MANA, &item_details_container, inventory->nb_mana_potions, unit_padding - 2);
            }
            break;

        case GO_BACK:
        default:
            break;
    }

    if (active_section == ITEMS){
        SDL_RenderPresent(game_window->renderer);
        return EXIT_SUCCESS;
    }

    display_actions(game_window->renderer, &actions_container, unit_padding - 2, active_action);

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}

int display_categories(SDL_Renderer *renderer,
                        int icon_size,
                        int padding,
                        section_options_t active_section,
                        category_options_t active_category) {
    const int categories_count = 5;
    SDL_Rect categories[5];

    for (int i = 0; i < categories_count; i++) {
        categories[i] = (SDL_Rect) {
                padding,
                i * icon_size + padding * (2 * i + 1),
                icon_size,
                icon_size
        };
    }

    if (draw_image_in_rectangle(renderer, categories[GO_BACK], "../assets/items_mgmt/image/go_back.png",
                                NORTH, false, ALIGN_START, ALIGN_START)) {
        return EXIT_FAILURE;
    }
    if (draw_image_in_rectangle(renderer, categories[WEAPONS], "../assets/items_mgmt/image/weapons.png",
                                NORTH, false, ALIGN_START, ALIGN_START)) {
        return EXIT_FAILURE;
    }
    if (draw_image_in_rectangle(renderer, categories[ARMORS], "../assets/items_mgmt/image/armors.png", NORTH,
                                false, ALIGN_START, ALIGN_START)) {
        return EXIT_FAILURE;
    }
    if (draw_image_in_rectangle(renderer, categories[HEALTH_POTIONS],
                                "../assets/items_mgmt/image/health_potions.png", NORTH, false, ALIGN_START,
                                ALIGN_START)) {
        return EXIT_FAILURE;
    }
    if (draw_image_in_rectangle(renderer, categories[MANA_POTIONS],
                                "../assets/items_mgmt/image/mana_potions.png", NORTH, false, ALIGN_START,
                                ALIGN_START)) {
        return EXIT_FAILURE;
    }

    if (active_section == CATEGORIES) {
        draw_thick_rect(categories[active_category], 2, (SDL_Color) {255, 255, 255, 255}, renderer);
    } else {
        draw_thick_rect(categories[active_category], 2, (SDL_Color) {255, 0, 0, 255}, renderer);
    }

    return EXIT_SUCCESS;
}

int display_items(SDL_Renderer *renderer,
                   SDL_Rect *items_container,
                   SDL_Rect *details_container,
                   item_types_t type, inventory_t *inventory,
                   int details_font_size,
                   unsigned short active_item,
                   section_options_t active_section) {
    unsigned int quantity = (type == ARMOR) ? inventory->nb_armors : (type == WEAPON) ? inventory->nb_weapons : 0;
    if (quantity == 0) {
        return EXIT_FAILURE;
    }

    SDL_Color white = (SDL_Color) {255, 255, 255, 255};
    SDL_Color red = (SDL_Color) {255, 0, 0, 255};

    const int items_count = 9;
    SDL_Rect *items = get_rectangle_grid(items_count, items_container);
    int first_item_to_print = (active_item / items_count) * items_count;

    display_scroll_indicator(renderer, items_container, details_font_size, items_count, quantity, first_item_to_print);

    char *details;
    switch (type) {
        case ARMOR:
            for (int i = 0; i < items_count; i++) {
                armor_t *armor_to_print = get_value_at_index(inventory->armors_head, first_item_to_print + i);
                if (!armor_to_print) {
                    break;
                }
                if (active_item % items_count == i) {
                    if (active_section == ITEMS) {
                        draw_thick_rect(items[i], 2, white, renderer);
                    } else {
                        draw_thick_rect(items[i], 2, red, renderer);
                    }
                }
                if (draw_image_in_rectangle(renderer, items[i], armor_to_print->image_path, NORTH, true, ALIGN_START,
                                            ALIGN_START)){
                    return EXIT_FAILURE;
                }
            }

            details = inventory_armor_details_to_string(get_value_at_index(inventory->armors_head, active_item));
            break;

        case WEAPON:
            for (int i = 0; i < items_count; i++) {
                weapon_t *weapon_to_print = get_value_at_index(inventory->weapons_head, first_item_to_print + i);
                if (!weapon_to_print) {
                    break;
                }
                if (active_item % items_count == i) {
                    if (active_section == ITEMS) {
                        draw_thick_rect(items[i], 2, white, renderer);
                    } else {
                        draw_thick_rect(items[i], 2, red, renderer);
                    }
                }
                if (draw_image_in_rectangle(renderer, items[i], weapon_to_print->image_path, NORTH, true, ALIGN_START,
                                            ALIGN_START)){
                    return EXIT_FAILURE;
                }
            }

            details = inventory_weapon_details_to_string(get_value_at_index(inventory->weapons_head, active_item));
            break;
    }

    print_text_in_rectangle(renderer, *details_container, details, white, ALIGN_START, ALIGN_START);

    return EXIT_SUCCESS;
}

int display_potions(SDL_Renderer *renderer,
                     potion_types_t type,
                     SDL_Rect *details_container,
                     unsigned int quantity,
                     int details_font_size) {
    char *details;
    switch (type) {
        case HEALTH:
            details = inventory_health_potions_details_to_string(quantity);
            break;

        case MANA:
            details = inventory_mana_potions_details_to_string(quantity);
            break;
    }

    print_text_in_rectangle(renderer, *details_container, details, (SDL_Color) {255, 255, 255, 255}, ALIGN_START, ALIGN_START);

    return EXIT_SUCCESS;
}

int display_actions(SDL_Renderer *renderer,
                    SDL_Rect *actions_container,
                    int font_size,
                    action_options_t active_action) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};

    char *actions[ACTIONS_COUNT] = {"Use", "Throw away"};

    SDL_Texture *cursor_texture = get_string_texture(
            renderer,
            ">",
            "../assets/PixelifySans-Regular.ttf",
            font_size,
            white
    );
    if (!cursor_texture) {
        return EXIT_FAILURE;
    }

    int cursor_width = 0;
    int cursor_height = 0;
    SDL_QueryTexture(cursor_texture, NULL, NULL, &cursor_width, &cursor_height);

    for (int i = 0; i < ACTIONS_COUNT; i++) {
        SDL_Texture *actions_texture = get_string_texture(
                renderer,
                actions[i],
                "../assets/PixelifySans-Regular.ttf",
                font_size,
                white
        );
        if (!actions_texture) {
            return EXIT_FAILURE;
        }

        int action_width = 0;
        int action_height = 0;
        SDL_QueryTexture(actions_texture, NULL, NULL, &action_width, &action_height);

        SDL_Rect action_container = (SDL_Rect) {
                actions_container->x + cursor_width + 8,
                actions_container->y + (cursor_height + 8) * i,
                action_width,
                action_height
        };

        SDL_RenderCopy(renderer, actions_texture, NULL, &action_container);
        SDL_DestroyTexture(actions_texture);
    }

    SDL_Rect cursor_container = (SDL_Rect) {
            actions_container->x,
            actions_container->y + (cursor_height + 8) * active_action,
            cursor_width,
            cursor_height
    };

    SDL_RenderCopy(renderer, cursor_texture, NULL, &cursor_container);
    SDL_DestroyTexture(cursor_texture);

    return EXIT_SUCCESS;
}

int display_nothing_to_see(SDL_Renderer *renderer, SDL_Rect *container) {
    print_text_in_rectangle(renderer, *container, "Nothing to see", (SDL_Color) {255, 255, 255, 255}, ALIGN_CENTER, ALIGN_CENTER);
    return EXIT_SUCCESS;
}

int display_scroll_indicator(SDL_Renderer *renderer,
                             SDL_Rect *container,
                             int font_size,
                             int items_count,
                             int quantity,
                             int first_item_to_print) {
    if (quantity > items_count) {
        SDL_Texture *arrow_texture = get_string_texture(
                renderer,
                "v",
                "../assets/PixelifySans-Regular.ttf",
                font_size,
                (SDL_Color) {255, 255, 255, 255}
        );
        if (!arrow_texture) {
            return EXIT_FAILURE;
        }

        int arrow_width = 0;
        int arrow_height = 0;
        SDL_QueryTexture(arrow_texture, NULL, NULL, &arrow_width, &arrow_height);

        if (first_item_to_print > 0) {
            SDL_Rect up_container = (SDL_Rect) {
                    container->x + container->w + 2,
                    container->y,
                    arrow_width,
                    arrow_height
            };

            SDL_Point up_center = (SDL_Point) {up_container.w / 2, up_container.h / 2};

            SDL_RenderCopyEx(renderer, arrow_texture, NULL, &up_container, 180, &up_center, SDL_FLIP_NONE);
        }

        if (first_item_to_print < quantity - items_count){
            SDL_Rect down_container = (SDL_Rect) {
                    container->x + container->w + 2,
                    container->y + container->h - arrow_height,
                    arrow_width,
                    arrow_height
            };

            SDL_RenderCopy(renderer, arrow_texture, NULL, &down_container);
        }
    }

    return EXIT_SUCCESS;
}