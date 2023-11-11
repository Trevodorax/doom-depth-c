#include "gui.h"
#include "../../../utils/utils.h"
#include "../../../ui_utils/sdl_utils/sdl_utils.h"
#include "../../../ui_utils/ui_utils.h"
#include "../../utils/inventory_utils.h"

// util functions
/**
 * @brief displays a go back button and a button for each of the inventory items categories :
 * weapons, armors, health potions and mana potions
 *
 * @param game_window the game window to use
 * @param icon_size the size of the categories' icons
 * @param padding the padding on the left of the icons
 * @param active_section the section of the user experience the user is in
 * @param active_category the current category the user has chosen or has their cursor on
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_categories_gui(game_window_t * game_window, int icon_size, int padding, section_options_t active_section, category_options_t active_category);

/**
 * @brief displays a grid of 9 items in the current category, and details on the item the cursor is currently on
 *
 * @param game_window the game window to use
 * @param items_container the rectangle in which to display the grid
 * @param details_container the rectangle in which to display the item details
 * @param type the type of item the cursor is on (armor or weapon)
 * @param player player that has the inventory where the items are in
 * @param details_font_size the font size for the item details
 * @param active_item the index of the item the cursor is currently on
 * @param active_section the section of the user experience the user is in
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_items_gui(game_window_t * game_window, rect_t * items_container, SDL_Rect details_container, item_types_t type, player_t *player, int details_font_size, unsigned short active_item, section_options_t active_section);

/**
 * @brief displays details on the selected heal_potion
 *
 * @param game_window the game window to use
 * @param type the type of heal_potion selected (health or mana)
 * @param details_container the rectangle in which to display the item details
 * @param quantity the quantity of selected potions in the inventory
 * @param details_font_size the font size for the heal_potion details
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_potions_gui(game_window_t * game_window, potion_types_t type, SDL_Rect details_container, unsigned int quantity);

/**
 * @brief displays the possible actions from which to choose: use or throw away
 *
 * @param game_window the game window to use
 * @param actions_container the rectangle in which to display the actions
 * @param font_size the font size for the actions
 * @param active_action the action the cursor is currently on
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_actions_gui(game_window_t *game_window, SDL_Rect actions_container, int font_size,
                        action_options_t active_action, bool can_be_used, bool can_be_unequipped);

/**
 * @brief displays "Nothing to see" in a given container
 *
 * @param game_window the game window to use
 * @param container the rectangle in which to display the message
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_nothing_to_see_gui(game_window_t * game_window, SDL_Rect container);

int display_inventory_gui(game_window_t * game_window,
                      player_t *player,
                      section_options_t active_section,
                      category_options_t active_category,
                      action_options_t active_action,
                      unsigned short active_item) {
    inventory_t * inventory = player->inventory;

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    SDL_RenderClear(game_window->renderer);

    draw_fill_rect(
            (SDL_Rect){0, 0, window_width, window_height},
            game_window->sdl_color_palette->background,
            game_window->renderer
    );

    int unit = (min(window_width, window_height) == window_height) ? window_height / 3 : window_width / 4;
    int unit_padding = unit / 10;
    int category_size = unit / 5;

    display_categories_gui(game_window, category_size, unit_padding, active_section, active_category);
    if (active_section == CATEGORIES) {
        return EXIT_SUCCESS;
    }

    rect_t items_container = {
            category_size + 2 * unit_padding,
            unit_padding,
            3 * unit - 2 * unit_padding,
            3 * unit - 2 * unit_padding
    };

    rect_t item_details_container = {
            category_size + 3 * unit + 2 * unit_padding,
            unit_padding,
            window_width - (items_container.x + 3 * unit),
            2 * unit +unit_padding
    };
    rect_t actions_container = {
            category_size + 3 * unit + 2 * unit_padding,
            unit_padding + 2 * unit,
            window_width - (category_size + 3 * unit) - unit_padding,
            unit
    };

    switch (active_category) {
        case WEAPONS:
            if (!inventory->nb_weapons) {
                display_nothing_to_see_gui(game_window, rect_to_SDL_Rect(items_container));
                return EXIT_SUCCESS;
            } else {
                display_items_gui(game_window, &items_container, rect_to_SDL_Rect(item_details_container), WEAPON, player, unit_padding - 2, active_item, active_section);
            }
            break;

        case ARMORS:
            if (!inventory->nb_armors) {
                display_nothing_to_see_gui(game_window, rect_to_SDL_Rect(items_container));
                return EXIT_SUCCESS;
            } else {
                display_items_gui(game_window, &items_container, rect_to_SDL_Rect(item_details_container), ARMOR, player, unit_padding - 2, active_item, active_section);
            }
            break;

        case HEALTH_POTIONS:
            if (!inventory->nb_health_potions) {
                display_nothing_to_see_gui(game_window, rect_to_SDL_Rect(items_container));
                return EXIT_SUCCESS;
            } else {
                display_potions_gui(game_window, HEALTH, rect_to_SDL_Rect(item_details_container), inventory->nb_health_potions);
            }
            break;

        case MANA_POTIONS:
            if (inventory->nb_mana_potions == 0) {
                display_nothing_to_see_gui(game_window, rect_to_SDL_Rect(items_container));
                return EXIT_SUCCESS;
            } else {
                display_potions_gui(game_window, MANA, rect_to_SDL_Rect(item_details_container), inventory->nb_mana_potions);
            }
            break;

        case GO_BACK:
        default:
            break;
    }

    if (active_section == ITEMS){
        return EXIT_SUCCESS;
    }

    if ((active_category == ARMORS && get_value_at_index(player->inventory->armors_head, active_item) == player->chosen_armor) ||
            (active_category == WEAPONS && get_value_at_index(player->inventory->weapons_head, active_item) == player->chosen_weapon)) {
        display_actions_gui(game_window, rect_to_SDL_Rect(actions_container), unit_padding - 2, active_action, false,
                            true);
    } else {
        display_actions_gui(game_window, rect_to_SDL_Rect(actions_container), unit_padding - 2, active_action, true, false);
    }

    return EXIT_SUCCESS;
}

int display_categories_gui(game_window_t * game_window,
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

    if (draw_image_in_rectangle(game_window->renderer, categories[GO_BACK], "../assets/items_mgmt/image/go_back.png",
                                NORTH, false, ALIGN_START, ALIGN_START)) {
        return EXIT_FAILURE;
    }
    if (draw_image_in_rectangle(game_window->renderer, categories[WEAPONS], "../assets/items_mgmt/image/weapons.png",
                                NORTH, false, ALIGN_START, ALIGN_START)) {
        return EXIT_FAILURE;
    }
    if (draw_image_in_rectangle(game_window->renderer, categories[ARMORS], "../assets/items_mgmt/image/armors.png", NORTH,
                                false, ALIGN_START, ALIGN_START)) {
        return EXIT_FAILURE;
    }
    if (draw_image_in_rectangle(game_window->renderer, categories[HEALTH_POTIONS],
                                "../assets/items_mgmt/image/health_potions.png", NORTH, false, ALIGN_START,
                                ALIGN_START)) {
        return EXIT_FAILURE;
    }
    if (draw_image_in_rectangle(game_window->renderer, categories[MANA_POTIONS],
                                "../assets/items_mgmt/image/mana_potions.png", NORTH, false, ALIGN_START,
                                ALIGN_START)) {
        return EXIT_FAILURE;
    }

    if (active_section == CATEGORIES) {
        draw_thick_rect(categories[active_category], 2, game_window->sdl_color_palette->text, game_window->renderer);
    } else {
        draw_thick_rect(categories[active_category], 2, game_window->sdl_color_palette->highlight, game_window->renderer);
    }

    return EXIT_SUCCESS;
}

int display_items_gui(game_window_t * game_window,
                  rect_t * items_container,
                  SDL_Rect details_container,
                  item_types_t type,
                  player_t *player,
                  int details_font_size,
                  unsigned short active_item,
                  section_options_t active_section) {
    inventory_t *inventory = player->inventory;
    unsigned int quantity = (type == ARMOR) ? inventory->nb_armors : (type == WEAPON) ? inventory->nb_weapons : 0;
    if (quantity == 0) {
        return EXIT_FAILURE;
    }

    const int items_count = 9;
    rect_t *items = get_rectangle_grid(items_count, items_container);
    int first_item_to_print = (active_item / items_count) * items_count;

    display_scroll_indicator_gui(game_window, rect_to_SDL_Rect(*items_container), details_font_size, items_count, (int) quantity, first_item_to_print);

    char *details;
    switch (type) {
        case ARMOR:
            for (int i = 0; i < items_count; i++) {
                armor_t *armor_to_print = get_value_at_index(inventory->armors_head, first_item_to_print + i);
                if (!armor_to_print) {
                    break;
                }
                if (armor_to_print == player->chosen_armor) {
                    draw_thick_rect(rect_to_SDL_Rect(items[i]), 2, game_window->sdl_color_palette->purple, game_window->renderer);
                }
                if (active_item % items_count == i) {
                    if (active_section == ITEMS) {
                        draw_thick_rect(rect_to_SDL_Rect(items[i]), 2, game_window->sdl_color_palette->text, game_window->renderer);
                    } else {
                        draw_thick_rect(rect_to_SDL_Rect(items[i]), 2, game_window->sdl_color_palette->highlight, game_window->renderer);
                    }
                }
                if (draw_image_in_rectangle(game_window->renderer, rect_to_SDL_Rect(items[i]), armor_to_print->image_path, NORTH, true, ALIGN_START,
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
                if (weapon_to_print == player->chosen_weapon) {
                    draw_thick_rect(rect_to_SDL_Rect(items[i]), 2, game_window->sdl_color_palette->purple, game_window->renderer);
                }
                if (active_item % items_count == i) {
                    if (active_section == ITEMS) {
                        draw_thick_rect(rect_to_SDL_Rect(items[i]), 2, game_window->sdl_color_palette->text, game_window->renderer);
                    } else {
                        draw_thick_rect(rect_to_SDL_Rect(items[i]), 2, game_window->sdl_color_palette->highlight, game_window->renderer);
                    }
                }
                if (draw_image_in_rectangle(game_window->renderer, rect_to_SDL_Rect(items[i]), weapon_to_print->image_path, NORTH, true, ALIGN_START,
                                            ALIGN_START)){
                    return EXIT_FAILURE;
                }
            }

            details = inventory_weapon_details_to_string(get_value_at_index(inventory->weapons_head, active_item));
            break;
    }

    print_text_in_rectangle(game_window->renderer, details_container, details, game_window->sdl_color_palette->text, ALIGN_START, ALIGN_START);

    return EXIT_SUCCESS;
}

int display_potions_gui(game_window_t * game_window,
                    potion_types_t type,
                    SDL_Rect details_container,
                    unsigned int quantity) {
    char *details;
    switch (type) {
        case HEALTH:
            details = inventory_health_potions_details_to_string(quantity);
            break;

        case MANA:
            details = inventory_mana_potions_details_to_string(quantity);
            break;
    }

    print_text_in_rectangle(game_window->renderer, details_container, details,
                            game_window->sdl_color_palette->text, ALIGN_START, ALIGN_START);

    return EXIT_SUCCESS;
}

#define ACTIONS_COUNT 3
int display_actions_gui(game_window_t *game_window, SDL_Rect actions_container, int font_size,
                        action_options_t active_action, bool can_be_used, bool can_be_unequipped) {
    char *actions[ACTIONS_COUNT] = {"Use", "Unequip", "Throw away"};

    SDL_Texture *cursor_texture = get_string_texture(
            game_window->renderer,
            ">",
            "../assets/PixelifySans-Regular.ttf",
            font_size,
            game_window->sdl_color_palette->highlight
    );
    if (!cursor_texture) {
        return EXIT_FAILURE;
    }

    int cursor_width = 0;
    int cursor_height = 0;
    SDL_QueryTexture(cursor_texture, NULL, NULL, &cursor_width, &cursor_height);

    for (int i = 0; i < ACTIONS_COUNT; i++) {
        SDL_Texture *actions_texture = get_string_texture(
                game_window->renderer,
                actions[i],
                "../assets/PixelifySans-Regular.ttf",
                font_size,
                ((i == USE && !can_be_used) || (i == UNEQUIP && !can_be_unequipped)) ? game_window->sdl_color_palette->disabled : game_window->sdl_color_palette->text
        );
        if (!actions_texture) {
            return EXIT_FAILURE;
        }

        int action_width = 0;
        int action_height = 0;
        SDL_QueryTexture(actions_texture, NULL, NULL, &action_width, &action_height);

        SDL_Rect action_container = (SDL_Rect) {
                actions_container.x + cursor_width + 8,
                actions_container.y + (cursor_height + 8) * i,
                action_width,
                action_height
        };

        SDL_RenderCopy(game_window->renderer, actions_texture, NULL, &action_container);
        SDL_DestroyTexture(actions_texture);
    }

    SDL_Rect cursor_container = (SDL_Rect) {
            actions_container.x,
            actions_container.y + (cursor_height + 8) * active_action,
            cursor_width,
            cursor_height
    };

    if ((can_be_used || active_action != USE) && (can_be_unequipped || active_action != UNEQUIP)) {
        SDL_RenderCopy(game_window->renderer, cursor_texture, NULL, &cursor_container);
        SDL_DestroyTexture(cursor_texture);
    }

    return EXIT_SUCCESS;
}

int display_nothing_to_see_gui(game_window_t * game_window, SDL_Rect container) {
    print_text_in_rectangle(game_window->renderer, container, "Nothing to see",
                            game_window->sdl_color_palette->text, ALIGN_CENTER, ALIGN_CENTER);
    return EXIT_SUCCESS;
}

int display_scroll_indicator_gui(game_window_t * game_window,
                             SDL_Rect container,
                             int font_size,
                             int items_count,
                             int quantity,
                             int first_item_to_print) {
    if (quantity > items_count) {
        SDL_Texture *arrow_texture = get_string_texture(
                game_window->renderer,
                "v",
                "../assets/PixelifySans-Regular.ttf",
                font_size,
                game_window->sdl_color_palette->text
        );
        if (!arrow_texture) {
            return EXIT_FAILURE;
        }

        int arrow_width = 0;
        int arrow_height = 0;
        SDL_QueryTexture(arrow_texture, NULL, NULL, &arrow_width, &arrow_height);

        if (first_item_to_print > 0) {
            SDL_Rect up_container = (SDL_Rect) {
                    container.x + container.w + 2,
                    container.y,
                    arrow_width,
                    arrow_height
            };

            SDL_Point up_center = (SDL_Point) {up_container.w / 2, up_container.h / 2};

            SDL_RenderCopyEx(game_window->renderer, arrow_texture, NULL, &up_container, 180, &up_center, SDL_FLIP_NONE);
        }

        if (first_item_to_print < quantity - items_count){
            SDL_Rect down_container = (SDL_Rect) {
                    container.x + container.w + 2,
                    container.y + container.h - arrow_height,
                    arrow_width,
                    arrow_height
            };

            SDL_RenderCopy(game_window->renderer, arrow_texture, NULL, &down_container);
        }
    }

    return EXIT_SUCCESS;
}
