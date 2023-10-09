#include "inventory_screen.h"
#include "../sdl_utils/sdl_utils.h"
#include "../utils/router.h"
#include "../utils/array.h"
#include "SDL_keycode.h"
#include "string.h"
#include "SDL_ttf.h"
#include "../utils/utils.h"
#include <stdlib.h>

int display_inventory(game_window_t *game_window, inventory_t *inventory, section_options active_section, category_options active_category, action_options active_action, unsigned short active_item);
int display_nothing_to_see(SDL_Renderer *renderer, int rect_x, int rect_y);
void handle_categories_input(SDL_Keycode keycode, bool *quit, section_options *active_section, category_options *active_category, unsigned short *active_item);
void handle_items_input(SDL_Keycode keycode, section_options *active_section, category_options *active_category, unsigned short *active_item, unsigned short category_items_count);
char *armor_details_to_string(armor_t *armor);
char *weapon_details_to_string(weapon_t *weapon);
char *health_potions_details_to_string(unsigned int quantity);
char *mana_potions_details_to_string(unsigned int quantity);

int inventory_screen(game_window_t *game_window, player_t *player) {
    if (!game_window) {
        printf("Cannot display inventory : no game window\n");
        return EXIT_FAILURE;
    }
    if (!player) {
        printf("Cannot display inventory : no player\n");
        return EXIT_FAILURE;
    }

    SDL_Event e;
    bool quit = false;
    section_options active_section = CATEGORIES;
    category_options active_category = WEAPONS;
    action_options active_action = USE;
    unsigned short active_item = 0;
    while (!quit){
        // TODO: link to next screens
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                return QUIT;
            }
            if (e.type == SDL_KEYDOWN){
                switch (active_section){
                    case CATEGORIES:
                        handle_categories_input(e.key.keysym.sym, &quit, &active_section, &active_category, &active_item);
                        break;

                    case ITEMS:
                        if (active_category == WEAPONS) {
                            handle_items_input(e.key.keysym.sym, &active_section, &active_category, &active_item, player->inventory->nb_weapons);
                        } else if (active_category == ARMORS) {
                            handle_items_input(e.key.keysym.sym, &active_section, &active_category, &active_item, player->inventory->nb_armors);
                        }
                        break;

                    case ACTIONS:
                        if (e.key.keysym.sym == SDLK_LEFT) {
                            if (active_category == MANA_POTIONS || active_category == HEALTH_POTIONS) {
                                active_section = CATEGORIES;
                            } else {
                               active_section--;
                            };
                        }
                        if (e.key.keysym.sym == SDLK_DOWN && active_action != THROW_AWAY) {
                            active_action++;
                        }
                        if (e.key.keysym.sym == SDLK_UP && active_action != USE) {
                            active_action--;
                        }
                        if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) {
                            if (active_action == USE) {
                                if (active_category == WEAPONS) {
                                    player->chosen_weapon = get_value_at_index(player->inventory->weaponsHead, active_item);
                                }
                                if (active_category == ARMORS) {
                                    player->chosen_armor = get_value_at_index(player->inventory->armorsHead, active_item);
                                }
                            }
                            if (active_action == THROW_AWAY) {
                                if (active_category == WEAPONS) {
                                    delete_node(&(player->inventory->weaponsHead), active_item);
                                }
                                if (active_category == ARMORS) {
                                    delete_node(&(player->inventory->armorsHead), active_item);
                                }
                            }
                        }
                        break;
                }
            }
        }
        display_inventory(game_window, player->inventory, active_section, active_category, active_action, active_item);
        SDL_Delay(50);
    }
    return EXIT_SUCCESS;
}

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

    const int categories_count = 5;
    SDL_Rect categories[5];
    for (int i = 0; i < categories_count; i++) {
        categories[i] = (SDL_Rect) {
            unit_padding,
            i * category_size + unit_padding * (2*i + 1),
            category_size,
            category_size
        };
    }

    draw_image_in_rectangle(game_window->renderer, categories[GO_BACK], "../assets/inventory_categories/go_back.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[WEAPONS], "../assets/inventory_categories/weapons.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[ARMORS], "../assets/inventory_categories/armors.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[HEALTH_POTIONS], "../assets/inventory_categories/health_potions.png", NORTH);
    draw_image_in_rectangle(game_window->renderer, categories[MANA_POTIONS], "../assets/inventory_categories/mana_potions.png", NORTH);

    if (active_section == CATEGORIES) {
        draw_thick_rect(categories[active_category], 2, (SDL_Color) {255, 255, 255, 255}, game_window->renderer);
        SDL_RenderPresent(game_window->renderer);
        return EXIT_SUCCESS;
    } else {
        draw_thick_rect(categories[active_category], 2, (SDL_Color) {255, 0, 0, 255}, game_window->renderer);
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
    SDL_Texture *item_details = NULL;

    const int items_count = 9;
    SDL_Rect *items = get_rectangle_grid(9, &items_container);

    int first_item_to_print = (active_item / items_count) * items_count;
    const char *details;
    switch (active_category) {
        case WEAPONS:
            if (inventory->nb_weapons == 0) {
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
                break;
            }
            for (int i = 0; i < items_count; i++) {
                weapon_t *weapon_to_print = get_value_at_index(inventory->weaponsHead, first_item_to_print + i);
                if (weapon_to_print == NULL) {
                    break;
                }
                if (active_item == i) {
                    draw_thick_rect(items[i], 2, (SDL_Color) {255, 255, 255, 255}, game_window->renderer);
                }
                draw_image_in_rectangle(game_window->renderer, items[i], weapon_to_print->image_path, NORTH);
            }
//            details = weapon_details_to_string(get_value_at_index(inventory->weaponsHead, active_item));
//            item_details = get_string_texture(game_window->renderer,
//                                              details,
//                                              "../assets/PixelifySans-Regular.ttf",
//                                              14,
//                                              (SDL_Color) {255, 255, 255, 255}
//            );
            break;

        case ARMORS:
            if (inventory->nb_armors == 0) {
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
                break;
            }
            for (int i = 0; i < items_count; i++) {
                armor_t *armor_to_print = get_value_at_index(inventory->armorsHead, first_item_to_print + i);
                if (armor_to_print == NULL) {
                    break;
                }
                if (active_item == i) {
                    draw_thick_rect(items[i], 2, (SDL_Color) {255, 255, 255, 255}, game_window->renderer);
                }
                draw_image_in_rectangle(game_window->renderer, items[i], armor_to_print->image_path, NORTH);
            }
            details = armor_details_to_string(get_value_at_index(inventory->armorsHead, active_item));
            item_details = get_string_texture(game_window->renderer,
                                                      details,
                                                      "../assets/PixelifySans-Regular.ttf",
                                                      14,
                                                      (SDL_Color) {255, 255, 255, 255}
            );
            break;

        case HEALTH_POTIONS:
            if (inventory->nb_health_potions == 0) {
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
                break;
            }
            details = health_potions_details_to_string(inventory->nb_health_potions);
            item_details = get_string_texture(game_window->renderer,
                                                      details,
                                                      "../assets/PixelifySans-Regular.ttf",
                                                      14,
                                                      (SDL_Color) {255, 255, 255, 255}
            );
            break;

        case MANA_POTIONS:
            if (inventory->nb_mana_potions == 0) {
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
                break;
            }
            details = mana_potions_details_to_string(inventory->nb_mana_potions);
            item_details = get_string_texture(game_window->renderer,
                                              details,
                                              "../assets/PixelifySans-Regular.ttf",
                                              14,
                                              (SDL_Color) {255, 255, 255, 255}
            );
            break;

        case GO_BACK:
        default:
            break;
    }

    int item_details_width = 0;
    int item_details_height = 0;
    SDL_QueryTexture(item_details, NULL, NULL, &item_details_width, &item_details_height);
    item_details_container.h = item_details_height;
    printf("item_details_container.h : %d\n", item_details_container.h);

    SDL_RenderCopy(game_window->renderer, item_details, NULL, &item_details_container);
    SDL_DestroyTexture(item_details);

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}

int display_nothing_to_see(SDL_Renderer *renderer, int rect_x, int rect_y) {
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

    int nothing_to_see_width = 0;
    int nothing_to_see_height = 0;
    SDL_QueryTexture(nothing_to_see_texture, NULL, NULL, &nothing_to_see_width, &nothing_to_see_height);

    SDL_Rect nothing_to_see_rect = (SDL_Rect) {
            rect_x,
            rect_y,
            nothing_to_see_width,
            nothing_to_see_height
    };

    SDL_RenderCopy(renderer, nothing_to_see_texture, NULL, &nothing_to_see_rect);
    SDL_DestroyTexture(nothing_to_see_texture);

    return EXIT_SUCCESS;
}

void handle_categories_input(SDL_Keycode keycode, bool *quit, section_options *active_section, category_options *active_category, unsigned short *active_item) {
    // navigating through the categories
    if (keycode == SDLK_DOWN && *active_category != MANA_POTIONS) {
    (*active_category)++;
    *active_item = 0;
    }
    if (keycode == SDLK_UP && *active_category != GO_BACK) {
    (*active_category)--;
    *active_item = 0;
    }
    // accessing the chosen category
    if (keycode == SDLK_RETURN || keycode == SDLK_KP_ENTER) {
    switch (*active_category) {
        case GO_BACK:
            *quit = true;
            return;

        case ARMORS:
        case WEAPONS:
            (*active_section)++;
            break;

        case MANA_POTIONS:
        case HEALTH_POTIONS:
            *active_section = ACTIONS;
            break;
        }
    }
}

void handle_items_input(SDL_Keycode keycode, section_options *active_section, category_options *active_category, unsigned short *active_item, unsigned short category_items_count) {
    if (keycode == SDLK_LEFT) {
        // going back to categories when pressing the left arrow on the left column
        if (*active_item % 3 == 0) {
            (*active_section)--;
        } else {
            (*active_item)--;
        }
    }
    if (keycode == SDLK_RIGHT) {
        if (*active_item % 3 != 2) {
            (*active_item)++;
        }
    }
    if (keycode == SDLK_DOWN) {
        if ((*active_category == WEAPONS || *active_category == ARMORS) && *active_item + 3 < category_items_count) {
            (*active_item) += 3;
        }
    }
    if (keycode == SDLK_UP) {
        if ((*active_category == WEAPONS && *active_item - 3 >= 0) ||
            (*active_category == ARMORS && *active_item - 3 >= 0)) {
            (*active_item) -= 3;
        }
    }
    if (keycode == SDLK_RETURN || keycode == SDLK_KP_ENTER) {
        (*active_section)++;
    }
}

char *armor_details_to_string(armor_t *armor) {
    char buffer[4];

    char *res = malloc(sizeof(char));
    res[0] = '\0';

    strcat(res, "Name : ");
    strcat(res, custom_strupr(armor->name));
    strcat(res, "\n");

    strcat(res, "Rarity : ");
    strcat(res, custom_itoa((int) armor->rarity, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Defense value : ");
    strcat(res, custom_itoa((int) armor->amount, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Uses left : ");
    strcat(res, custom_itoa((int) armor->uses, buffer, 10));
    strcat(res, " out of ");
    strcat(res, custom_itoa((int) armor->max_uses, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Cost : ");
    strcat(res, custom_itoa((int) armor->cost, buffer, 10));

    return res;
}

char *weapon_details_to_string(weapon_t *weapon) {
    char buffer[4];

    char *res = malloc(sizeof(char));
    res[0] = '\0';

    strcat(res, "Name : ");
    strcat(res, custom_strupr(weapon->name));
    strcat(res, "\n");

    strcat(res, "Rarity : ");
    strcat(res, custom_itoa((int) weapon->rarity, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Attacks per turn : ");
    strcat(res, custom_itoa((int) weapon->attacks_per_turn, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Attack range : ");
    strcat(res, custom_itoa((int) weapon->min_attack, buffer, 10));
    strcat(res, " - ");
    strcat(res, custom_itoa((int) weapon->max_attack, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Uses left : ");
    strcat(res, custom_itoa((int) weapon->uses, buffer, 10));
    strcat(res, " out of ");
    strcat(res, custom_itoa((int) weapon->max_uses, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Cost : ");
    strcat(res, custom_itoa((int) weapon->cost, buffer, 10));

    return res;
}

char *health_potions_details_to_string(unsigned int quantity) {
    char buffer[4];

    char *res = malloc(sizeof(char));
    res[0] = '\0';

    strcat(res, "Name : HEALTH POTION\n");
    strcat(res, "Healing power : 20\n");
    strcat(res, "In stock : ");
    strcat(res, custom_itoa((int) quantity, buffer, 10));

    return res;
}

char *mana_potions_details_to_string(unsigned int quantity) {
    char buffer[4];

    char *res = malloc(sizeof(char));
    res[0] = '\0';

    strcat(res, "Name : MANA POTION\n");
    strcat(res, "Mana value : 20\n");
    strcat(res, "In stock : ");
    strcat(res, custom_itoa((int) quantity, buffer, 10));

    return res;
}