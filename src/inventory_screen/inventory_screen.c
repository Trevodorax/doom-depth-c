#include "inventory_screen.h"
#include "../utils/router.h"
#include "SDL_keycode.h"
#include "display/display.h"

void handle_categories_input(SDL_Keycode keycode, bool *quit, section_options *active_section, category_options *active_category, unsigned short *active_item);
void handle_items_input(SDL_Keycode keycode, section_options *active_section, category_options active_category, unsigned short *active_item, unsigned short category_items_count);
void handle_actions_input(SDL_Keycode keycode, player_t *player, section_options *active_section, category_options active_category, unsigned short active_item, action_options *active_action);

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
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                return QUIT_GAME;
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
    // going back to categories when pressing the left arrow on the left column
    if (keycode == SDLK_LEFT) {
        if (*active_item % 3 == 0) {
            (*active_section)--;
        } else {
            (*active_item)--;
        }
    }

    // moving in the items grid
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

    // choosing an item
    if (keycode == SDLK_RETURN || keycode == SDLK_KP_ENTER) {
        (*active_section)++;
    }
}

void handle_actions_input(SDL_Keycode keycode, player_t *player, section_options *active_section, category_options active_category, unsigned short active_item, action_options *active_action) {
    // going back to previous section when pressing the left arrow on the left column
    if (keycode == SDLK_LEFT) {
        if (active_category == MANA_POTIONS || active_category == HEALTH_POTIONS) {
            *active_section = CATEGORIES;
        } else {
            (*active_section)--;
        }
    }

    // moving through actions
    if (keycode == SDLK_DOWN && *active_action != THROW_AWAY) {
        (*active_action)++;
    }
    if (keycode == SDLK_UP && *active_action != USE) {
        (*active_action)--;
    }

    // TODO : add actions for potions
    // choosing action
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