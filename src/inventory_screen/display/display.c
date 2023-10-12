#include "display.h"
#include "../inventory_utils/inventory_utils.h"

int display_inventory(game_window_t *game_window, inventory_t *inventory, section_options active_section, category_options active_category, action_options active_action, unsigned short active_item) {
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    SDL_RenderClear(game_window->renderer);

    draw_fill_rect(
            (SDL_Rect){0, 0, window_width, window_height},
            (SDL_Color){0, 0, 0, 255},
            game_window->renderer
    );

    int unit = min(window_width, window_height) / 3;
    int unit_padding = unit / 10;
    int category_size = unit / 5;

    if (!display_categories(game_window->renderer, category_size, unit_padding, active_section, active_category)){
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
            category_size + 3 * unit,
            unit_padding,
            window_width - (category_size + 3 * unit) - unit_padding,
            0
    };
    SDL_Rect actions_container = (SDL_Rect) {
            category_size + 3 * unit,
            unit_padding + 2 * unit,
            window_width - (category_size + 3 * unit) - unit_padding,
            0
    };

    switch (active_category) {
        case WEAPONS:
            if (!display_items(game_window->renderer, &items_container, &item_details_container, WEAPON, inventory, unit_padding, active_item, active_section)) {
                display_nothing_to_see(game_window->renderer, &items_container);
                break;
            }
            break;

        case ARMORS:
            if (!display_items(game_window->renderer, &items_container, &item_details_container, ARMOR, inventory, unit_padding, active_item, active_section)) {
                display_nothing_to_see(game_window->renderer, &items_container);
                break;
            }
            break;

        case HEALTH_POTIONS:
            if (!display_potions(game_window->renderer, HEALTH, &item_details_container, inventory->nb_health_potions, unit_padding)){
                display_nothing_to_see(game_window->renderer, &items_container);
            }
            break;

        case MANA_POTIONS:
            if (!display_potions(game_window->renderer, MANA, &item_details_container, inventory->nb_mana_potions, unit_padding)){
                display_nothing_to_see(game_window->renderer, &items_container);
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

    display_actions(game_window->renderer, &actions_container, unit_padding, active_action);

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}

bool display_categories(SDL_Renderer *renderer, int icon_size, int padding, section_options active_section, category_options active_category) {
    const int categories_count = 5;
    SDL_Rect categories[5];

    for (int i = 0; i < categories_count; i++) {
        categories[i] = (SDL_Rect) {
                padding,
                i * icon_size + padding * (2*i + 1),
                icon_size,
                icon_size
        };
    }

    draw_image_in_rectangle(renderer, categories[GO_BACK], "../assets/inventory_categories/go_back.png", NORTH);
    draw_image_in_rectangle(renderer, categories[WEAPONS], "../assets/inventory_categories/weapons.png", NORTH);
    draw_image_in_rectangle(renderer, categories[ARMORS], "../assets/inventory_categories/armors.png", NORTH);
    draw_image_in_rectangle(renderer, categories[HEALTH_POTIONS], "../assets/inventory_categories/health_potions.png", NORTH);
    draw_image_in_rectangle(renderer, categories[MANA_POTIONS], "../assets/inventory_categories/mana_potions.png", NORTH);

    if (active_section == CATEGORIES) {
        draw_thick_rect(categories[active_category], 2, (SDL_Color) {255, 255, 255, 255}, renderer);
        return false;
    } else {
        draw_thick_rect(categories[active_category], 2, (SDL_Color) {255, 0, 0, 255}, renderer);
        return true;
    }
}

bool display_items(SDL_Renderer *renderer, SDL_Rect *items_container, SDL_Rect *details_container, item_types type, inventory_t *inventory, int details_font_size, unsigned short active_item, section_options active_section) {
    unsigned int quantity = (type == ARMOR) ? inventory->nb_armors : (type == WEAPON) ? inventory->nb_weapons : 0;
    if (quantity == 0) {
        return false;
    }

    SDL_Color white = (SDL_Color) {255, 255, 255, 255};
    SDL_Color red = (SDL_Color) {255, 0, 0, 255};

    const int items_count = 9;
    SDL_Rect *items = get_rectangle_grid(9, items_container);
    int first_item_to_print = (active_item / items_count) * items_count;

    // TODO : create scrollbar or indicator to tell the user they can scroll if they have more than 9 items in the category
    const char *details;
    switch (type) {
        case ARMOR:
            for (int i = 0; i < items_count; i++) {
                armor_t *armor_to_print = get_value_at_index(inventory->armorsHead, first_item_to_print + i);
                if (armor_to_print == NULL) {
                    break;
                }
                if (active_item == i) {
                    if (active_section == ITEMS) {
                        draw_thick_rect(items[i], 2, white, renderer);
                    } else {
                        draw_thick_rect(items[i], 2, red, renderer);
                    }
                }
                draw_image_in_rectangle(renderer, items[i], armor_to_print->image_path, NORTH);
            }

            details = armor_details_to_string(get_value_at_index(inventory->weaponsHead, active_item));
            break;

        case WEAPON:
            for (int i = 0; i < items_count; i++) {
                weapon_t *weapon_to_print = get_value_at_index(inventory->weaponsHead, first_item_to_print + i);
                if (weapon_to_print == NULL) {
                    break;
                }
                if (active_item == i) {
                    if (active_section == ITEMS) {
                        draw_thick_rect(items[i], 2, white, renderer);
                    } else {
                        draw_thick_rect(items[i], 2, red, renderer);
                    }
                }
                draw_image_in_rectangle(renderer, items[i], weapon_to_print->image_path, NORTH);
            }

            details = weapon_details_to_string(get_value_at_index(inventory->weaponsHead, active_item));
            break;
    }

    SDL_Texture *details_texture = get_string_texture(
            renderer,
            details,
            "../assets/PixelifySans-Regular.ttf",
            details_font_size,
            (SDL_Color) {255, 255, 255, 255}
    );
    if (!details_texture) {
        return false;
    }

    SDL_QueryTexture(details_texture, NULL, NULL, &(details_container->w), &(details_container->h));

    SDL_RenderCopy(renderer, details_texture, NULL, details_container);
    SDL_DestroyTexture(details_texture);

    return true;
}

bool display_potions(SDL_Renderer *renderer, potion_types type, SDL_Rect *details_container, unsigned int quantity, int details_font_size) {
    if (quantity == 0) {
        return false;
    }

    const char *details;
    switch (type) {
        case HEALTH:
            details = health_potions_details_to_string(quantity);
            break;

        case MANA:
            details = mana_potions_details_to_string(quantity);
            break;
    }

    SDL_Texture *details_texture = get_string_texture(
            renderer,
            details,
            "../assets/PixelifySans-Regular.ttf",
            details_font_size,
            (SDL_Color) {255, 255, 255, 255}
    );
    if (!details_texture) {
        return false;
    }

    SDL_QueryTexture(details_texture, NULL, NULL, &(details_container->w), &(details_container->h));

    SDL_RenderCopy(renderer, details_texture, NULL, details_container);
    SDL_DestroyTexture(details_texture);

    return true;
}

int display_actions(SDL_Renderer *renderer, SDL_Rect *actions_container, int font_size, action_options active_action) {
    SDL_Color white = (SDL_Color) {255, 255, 255, 255};

    const int actions_count = 2;
    char *actions[2] = {"Use", "Throw away"};

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

    for (int i = 0; i < actions_count; i++) {
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
    SDL_Texture *nothing_to_see_texture = get_string_texture(
            renderer,
            "Nothing to see here",
            "../assets/PixelifySans-Regular.ttf",
            24,
            (SDL_Color) {255, 255, 255, 255}
    );
    if (!nothing_to_see_texture) {
        return EXIT_FAILURE;
    }

    SDL_QueryTexture(nothing_to_see_texture, NULL, NULL, &(container->w), &(container->h));

    SDL_RenderCopy(renderer, nothing_to_see_texture, NULL, container);
    SDL_DestroyTexture(nothing_to_see_texture);

    return EXIT_SUCCESS;
}