#include "cli.h"
#include "../../../utils/utils.h"
#include "../../../ui_utils/cli_utils/ascii_art/ascii_art.h"
#include "../../../ui_utils/ui_utils.h"
#include "../../utils/inventory_utils.h"

int display_categories_cli(game_window_t * game_window, rect_t container, section_options_t active_section, category_options_t active_category);
int display_items_cli(game_window_t * game_window, rect_t *items_container, rect_t details_container, item_types_t type,
                      inventory_t *inventory, unsigned short active_item, section_options_t active_section);
int display_potions_cli(game_window_t * game_window, potion_types_t type, rect_t details_container, unsigned int quantity);
int display_actions_cli(game_window_t * game_window, rect_t actions_container, action_options_t active_action);
int display_nothing_to_see_cli(game_window_t * game_window, rect_t container);

int display_inventory_cli(game_window_t * game_window,
                          inventory_t * inventory,
                          section_options_t active_section,
                          category_options_t active_category,
                          action_options_t active_action,
                          unsigned short active_item) {
    int cli_width = (int)game_window->matrix->nb_cols;
    int cli_height = (int)game_window->matrix->nb_rows;

    cli_render_clear(game_window->matrix, (cli_char_t){' ', game_window->cli_color_palette->text});

    rect_t categories_container = {
            0,
            0,
            cli_width / 8,
            cli_height
    };

    int details_container_width = cli_width / 8;
    rect_t item_details_container = {
            cli_width - details_container_width,
            0,
            details_container_width,
            cli_height * 2 / 3
    };
    rect_t actions_container = {
            item_details_container.x,
            item_details_container.y + item_details_container.h,
            details_container_width,
            cli_height / 3
    };

    rect_t items_container = {
            categories_container.x + categories_container.w,
            0,
            cli_width - categories_container.w - item_details_container.w,
            cli_height
    };


    /*
    cli_draw_fill_rect(cli_matrix, categories_container, (cli_char_t){'#', CYAN});
    cli_draw_fill_rect(cli_matrix, items_container, (cli_char_t){'#', BLUE});
    cli_draw_fill_rect(cli_matrix, actions_container, (cli_char_t){'#', BLACK});
    cli_draw_fill_rect(cli_matrix, item_details_container, (cli_char_t){'#', RED});
     */

    display_categories_cli(game_window, categories_container, active_section, active_category);
    if (active_section == CATEGORIES) {
        return EXIT_SUCCESS;
    }

    switch (active_category) {
        case WEAPONS:
            if (!inventory->nb_weapons) {
                display_nothing_to_see_cli(game_window, items_container);
            } else {
                display_items_cli(game_window, &items_container, item_details_container, WEAPON, inventory, active_item,
                                  active_section);
            }
            break;

        case ARMORS:
            if (!inventory->nb_armors) {
                display_nothing_to_see_cli(game_window, items_container);
            } else {
                display_items_cli(game_window, &items_container, item_details_container, ARMOR, inventory, active_item,
                                  active_section);
            }
            break;

        case HEALTH_POTIONS:
            if (!inventory->nb_health_potions) {
                display_nothing_to_see_cli(game_window, items_container);
            } else {
                display_potions_cli(game_window, HEALTH, item_details_container, inventory->nb_health_potions);
            }
            break;

        case MANA_POTIONS:
            if (inventory->nb_mana_potions == 0) {
                display_nothing_to_see_cli(game_window, items_container);
            } else {
                display_potions_cli(game_window, MANA, item_details_container, inventory->nb_mana_potions);
            }
            break;

        case GO_BACK:
        default:
            break;
    }

    if (active_section == ITEMS){
        return EXIT_SUCCESS;
    }

    display_actions_cli(game_window, actions_container, active_action);

    return EXIT_SUCCESS;
}


int display_categories_cli(game_window_t * game_window,
                           rect_t container,
                           section_options_t active_section,
                           category_options_t active_category) {
    const int categories_count = 5;
    rect_t * categories = get_rectangle_layout(categories_count, &container, VERTICAL, 2);

    for (int i = 0; i < categories_count; ++i) {
        // The images are replaced with text labels for CLI version.
        char *ascii_path;
        switch (i) {
            case GO_BACK: {
                ascii_path = "../assets/items_mgmt/ascii/go_back.asciiart";
                break;
            }
            case WEAPONS: {
                ascii_path = "../assets/items_mgmt/ascii/weapons.asciiart";
                break;
            }
            case ARMORS: {
                ascii_path = "../assets/items_mgmt/ascii/armors.asciiart";
                break;
            }
            case HEALTH_POTIONS: {
                ascii_path = "../assets/items_mgmt/ascii/health_potions.asciiart";
                break;
            }
            case MANA_POTIONS: {
                ascii_path = "../assets/items_mgmt/ascii/mana_potions.asciiart";
                break;
            }
            default: {
                ascii_path = "Unknown";
                break;
            }
        }
        rect_t ascii_art_container = {
                categories[i].x + 1,
                categories[i].y + 1,
                categories[i].w - 2,
                categories[i].h - 2
        };
        if (print_ascii_art_in_rectangle(game_window->matrix, ascii_path, ascii_art_container, ALIGN_CENTER, ALIGN_CENTER, game_window->cli_color_palette->text) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }

    if (active_section == CATEGORIES) {
        cli_draw_stroke_rect(game_window->matrix, categories[active_category], (cli_char_t){'#', game_window->cli_color_palette->text});
    } else {
        cli_draw_stroke_rect(game_window->matrix, categories[active_category], (cli_char_t){'#', game_window->cli_color_palette->highlight});
    }

    return EXIT_SUCCESS;
}


int display_items_cli(game_window_t * game_window, rect_t * items_container, rect_t details_container, item_types_t type,
                      inventory_t * inventory, unsigned short active_item, section_options_t active_section) {
    unsigned int quantity = (type == ARMOR) ? inventory->nb_armors : (type == WEAPON) ? inventory->nb_weapons : 0;
    if (quantity == 0) {
        return EXIT_FAILURE;
    }

    const int items_count = 9;
    rect_t *items = get_rectangle_layout(items_count, items_container, GRID, 2);
    int first_item_to_print = (active_item / items_count) * items_count;

    // TODOSOMEDAY: find a replacement for this
    // display_scroll_indicator_gui(renderer, rect_to_SDL_Rect(*items_container), details_font_size, items_count, quantity, first_item_to_print);

    char * details;
    switch (type) {
        case ARMOR:
            for (int i = 0; i < items_count; i++) {
                armor_t *armor_to_print = get_value_at_index(inventory->armors_head, first_item_to_print + i);
                if (!armor_to_print) {
                    break;
                }
                if (active_item % items_count == i) {
                    cli_draw_stroke_rect(game_window->matrix, items[i],
                     (cli_char_t){'#', active_section == ITEMS ? game_window->cli_color_palette->text : game_window->cli_color_palette->highlight});
                }
                rect_t ascii_art_container = {
                        items[i].x + 1,
                        items[i].y + 1,
                        items[i].w - 2,
                        items[i].h - 2
                };
                if (print_ascii_art_in_rectangle(game_window->matrix, armor_to_print->ascii_path, ascii_art_container, ALIGN_CENTER,ALIGN_CENTER, game_window->cli_color_palette->text) == EXIT_FAILURE){
                    return EXIT_FAILURE;
                }
            }

            details = inventory_armor_details_to_string(get_value_at_index(inventory->armors_head, active_item));
            break;

        case WEAPON:
            for (int i = 0; i < items_count; i++) {
                weapon_t * weapon_to_print = get_value_at_index(inventory->weapons_head, first_item_to_print + i);
                if (!weapon_to_print) {
                    break;
                }
                if (active_item % items_count == i) {
                    cli_draw_stroke_rect(game_window->matrix, items[i],
                     (cli_char_t){'#', active_section == ITEMS ? game_window->cli_color_palette->text : game_window->cli_color_palette->highlight});
                }
                rect_t ascii_art_container = {
                        items[i].x + 1,
                        items[i].y + 1,
                        items[i].w - 2,
                        items[i].h - 2
                };
                if (print_ascii_art_in_rectangle(game_window->matrix, weapon_to_print->ascii_path, ascii_art_container, ALIGN_CENTER,ALIGN_CENTER, game_window->cli_color_palette->text)){
                    return EXIT_FAILURE;
                }
            }

            details = inventory_weapon_details_to_string(get_value_at_index(inventory->weapons_head, active_item));
            break;
    }

    cli_print_text_in_rectangle(game_window->matrix, details_container, details,
                                game_window->cli_color_palette->text, ALIGN_START, ALIGN_START, TINY_TEXT);

    return EXIT_SUCCESS;
}

int display_potions_cli(game_window_t * game_window,
                        potion_types_t type,
                        rect_t details_container,
                        unsigned int quantity) {
    char * details;
    switch (type) {
        case HEALTH:
            details = inventory_health_potions_details_to_string(quantity);
            break;

        case MANA:
            details = inventory_mana_potions_details_to_string(quantity);
            break;
    }

    cli_print_text_in_rectangle(game_window->matrix, details_container, details,
                                game_window->cli_color_palette->text, ALIGN_START, ALIGN_START, TINY_TEXT);

    return EXIT_SUCCESS;
}

#define ACTIONS_COUNT 2
int display_actions_cli(game_window_t * game_window,
                        rect_t actions_container,
                        action_options_t active_action) {
    char *actions[ACTIONS_COUNT] = {"Use", "Throw away"};
    int cursor_size = 5;

    for (int i = 0; i < ACTIONS_COUNT; i++) {
        rect_t action_container = {
                actions_container.x + cursor_size + 8,
                actions_container.y + (cursor_size + 8) * i,
                actions_container.w,
                cursor_size
        };

        cli_print_text_in_rectangle(game_window->matrix, action_container, actions[i],
                                    game_window->cli_color_palette->text, ALIGN_START, ALIGN_CENTER, TINY_TEXT);
    }

    rect_t cursor_container = {
            actions_container.x,
            actions_container.y + (cursor_size + 8) * active_action,
            cursor_size,
            cursor_size
    };

    cli_print_text_in_rectangle(game_window->matrix, cursor_container, ">",
                                game_window->cli_color_palette->text, ALIGN_CENTER, ALIGN_CENTER, TINY_TEXT);

    return EXIT_SUCCESS;
}

int display_nothing_to_see_cli(game_window_t * game_window, rect_t container) {
    cli_print_text_in_rectangle(game_window->matrix, container, "Nothing to see",
                                game_window->cli_color_palette->text, ALIGN_CENTER, ALIGN_CENTER, SMALL_TEXT);
    return EXIT_SUCCESS;
}

/*
int display_scroll_indicator_gui(SDL_Renderer * renderer,
                                 SDL_Rect container,
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
                    container.x + container.w + 2,
                    container.y,
                    arrow_width,
                    arrow_height
            };

            SDL_Point up_center = (SDL_Point) {up_container.w / 2, up_container.h / 2};

            SDL_RenderCopyEx(renderer, arrow_texture, NULL, &up_container, 180, &up_center, SDL_FLIP_NONE);
        }

        if (first_item_to_print < quantity - items_count){
            SDL_Rect down_container = (SDL_Rect) {
                    container.x + container.w + 2,
                    container.y + container.h - arrow_height,
                    arrow_width,
                    arrow_height
            };

            SDL_RenderCopy(renderer, arrow_texture, NULL, &down_container);
        }
    }

    return EXIT_SUCCESS;
}
 */