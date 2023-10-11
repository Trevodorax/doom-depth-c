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
bool display_categories(SDL_Renderer *renderer, int icon_size, int padding, section_options active_section, category_options active_category);
bool display_items(SDL_Renderer *renderer, SDL_Rect *items_container, SDL_Rect *details_container, item_types type, inventory_t *inventory, int details_font_size, unsigned short active_item, section_options active_section);
bool display_potions(SDL_Renderer *renderer, potion_types type, SDL_Rect *details_container, unsigned int quantity, int details_font_size);
int display_actions(SDL_Renderer *renderer, SDL_Rect *actions_container, int font_size, action_options active_action);
int display_nothing_to_see(SDL_Renderer *renderer, int rect_x, int rect_y);
void handle_categories_input(SDL_Keycode keycode, bool *quit, section_options *active_section, category_options *active_category, unsigned short *active_item);
void handle_items_input(SDL_Keycode keycode, section_options *active_section, category_options active_category, unsigned short *active_item, unsigned short category_items_count);
void handle_actions_input(SDL_Keycode keycode, player_t *player, section_options *active_section, category_options active_category, unsigned short active_item, action_options *active_action);
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
                            handle_items_input(e.key.keysym.sym, &active_section, active_category, &active_item, player->inventory->nb_weapons);
                        } else if (active_category == ARMORS) {
                            handle_items_input(e.key.keysym.sym, &active_section, active_category, &active_item, player->inventory->nb_armors);
                        }
                        break;

                    case ACTIONS:
                        handle_actions_input(e.key.keysym.sym, player, &active_section, active_category, active_item, &active_action);
                        break;
                }
            }
        }
        display_inventory(game_window, player->inventory, active_section, active_category, active_action, active_item);
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
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
                break;
            }
            break;

        case ARMORS:
            if (!display_items(game_window->renderer, &items_container, &item_details_container, ARMOR, inventory, unit_padding, active_item, active_section)) {
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
                break;
            }
            break;

        case HEALTH_POTIONS:
            if (!display_potions(game_window->renderer, HEALTH, &item_details_container, inventory->nb_health_potions, unit_padding)){
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
            }
            break;

        case MANA_POTIONS:
            if (!display_potions(game_window->renderer, MANA, &item_details_container, inventory->nb_mana_potions, unit_padding)){
                display_nothing_to_see(game_window->renderer, category_size + 2 * unit_padding, unit_padding);
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

void handle_items_input(SDL_Keycode keycode, section_options *active_section, category_options active_category, unsigned short *active_item, unsigned short category_items_count) {
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
        if ((active_category == WEAPONS || active_category == ARMORS) && *active_item + 3 < category_items_count) {
            (*active_item) += 3;
        }
    }
    if (keycode == SDLK_UP) {
        if ((active_category == WEAPONS && *active_item - 3 >= 0) ||
            (active_category == ARMORS && *active_item - 3 >= 0)) {
            (*active_item) -= 3;
        }
    }
    if (keycode == SDLK_RETURN || keycode == SDLK_KP_ENTER) {
        (*active_section)++;
    }
}

void handle_actions_input(SDL_Keycode keycode, player_t *player, section_options *active_section, category_options active_category, unsigned short active_item, action_options *active_action) {
    if (keycode == SDLK_LEFT) {
        if (active_category == MANA_POTIONS || active_category == HEALTH_POTIONS) {
            *active_section = CATEGORIES;
        } else {
            (*active_section)--;
        };
    }
    if (keycode == SDLK_DOWN && *active_action != THROW_AWAY) {
        (*active_action)++;
    }
    if (keycode == SDLK_UP && *active_action != USE) {
        (*active_action)--;
    }
    if (keycode == SDLK_RETURN || keycode == SDLK_KP_ENTER) {
        if (*active_action == USE) {
            if (active_category == WEAPONS) {
                player->chosen_weapon = get_value_at_index(player->inventory->weaponsHead, active_item);
                *active_section = ITEMS;
            }
            if (active_category == ARMORS) {
                player->chosen_armor = get_value_at_index(player->inventory->armorsHead, active_item);
                *active_section = ITEMS;
            }
        }
        if (*active_action == THROW_AWAY) {
            if (active_category == WEAPONS) {
                delete_node(&(player->inventory->weaponsHead), active_item);
                *active_section = ITEMS;
            }
            if (active_category == ARMORS) {
                delete_node(&(player->inventory->armorsHead), active_item);
                *active_section = ITEMS;
            }
        }
    }
}

char *armor_details_to_string(armor_t *armor) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
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

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}

char *weapon_details_to_string(weapon_t *weapon) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
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

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}

char *health_potions_details_to_string(unsigned int quantity) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name : HEALTH POTION\n");
    strcat(res, "Healing power : 20\n");
    strcat(res, "In stock : ");
    strcat(res, custom_itoa((int) quantity, buffer, 10));

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}

char *mana_potions_details_to_string(unsigned int quantity) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name : MANA POTION\n");
    strcat(res, "Mana value : 20\n");
    strcat(res, "In stock : ");
    strcat(res, custom_itoa((int) quantity, buffer, 10));

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}