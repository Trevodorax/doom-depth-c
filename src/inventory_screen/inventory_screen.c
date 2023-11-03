#include "inventory_screen.h"
#include "../utils/router.h"
#include "display/display.h"
#include "../event/event.h"

void handle_categories_input(event_t event, bool *quit, section_options_t *active_section, category_options_t *active_category, unsigned short *active_item);
void handle_items_input(event_t event, section_options_t *active_section, category_options_t active_category, unsigned short *active_item, unsigned short category_items_count);
void handle_actions_input(event_t event, player_t *player, section_options_t *active_section, category_options_t active_category, unsigned short active_item, action_options_t *active_action);

int inventory_screen(game_window_t *game_window, player_t *player) {
    if (!game_window) {
        printf("Cannot display inventory : no game window\n");
        return EXIT_FAILURE;
    }
    if (!player) {
        printf("Cannot display inventory : no player\n");
        return EXIT_FAILURE;
    }

    event_t event;
    bool quit = false;
    section_options_t active_section = CATEGORIES;
    category_options_t active_category = WEAPONS;
    action_options_t active_action = USE;
    unsigned short active_item = 0;
    while (!quit){
        while (get_event(game_window->ui_type, &event)) {
            switch (event) {

                case QUIT:
                    return QUIT_GAME;
                case Z_KEY:
                case Q_KEY:
                case S_KEY:
                case D_KEY:
                case ENTER_KEY:
                    switch (active_section){
                        case CATEGORIES:
                            handle_categories_input(event, &quit, &active_section, &active_category, &active_item);
                            if (quit) {
                                return QUIT_GAME;
                            }
                            break;

                        case ITEMS:
                            if (active_category == WEAPONS) {
                                handle_items_input(event, &active_section, active_category, &active_item, player->inventory->nb_weapons);
                            } else if (active_category == ARMORS) {
                                handle_items_input(event, &active_section, active_category, &active_item, player->inventory->nb_armors);
                            }
                            break;

                        case ACTIONS:
                            handle_actions_input(event, player, &active_section, active_category, active_item, &active_action);
                            break;

                        case CONFIRM:
                            return EXIT_FAILURE;
                    }
                    break;
                case UNKNOWN_EVENT:
                default:
                    break;
            }
        }
        display_inventory(game_window, player->inventory, active_section, active_category, active_action, active_item);
    }
    return EXIT_SUCCESS;
}

void handle_categories_input(event_t event, bool *quit, section_options_t *active_section, category_options_t *active_category, unsigned short *active_item) {
    // navigating through the categories
    if (event == S_KEY && *active_category != MANA_POTIONS) {
    (*active_category)++;
    *active_item = 0;
    }
    if (event == Z_KEY && *active_category != GO_BACK) {
    (*active_category)--;
    *active_item = 0;
    }
    // accessing the chosen category
    if (event == ENTER_KEY) {
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

void handle_items_input(event_t event, section_options_t *active_section, category_options_t active_category, unsigned short *active_item, unsigned short category_items_count) {
    // going back to categories when pressing the left arrow on the left column
    if (event == Q_KEY) {
        if (*active_item % 3 == 0) {
            (*active_section)--;
        } else {
            (*active_item)--;
        }
    }

    // moving in the items grid
    if (event == D_KEY) {
        if (*active_item % 3 != 2 && *active_item < category_items_count - 1) {
            (*active_item)++;
        }
    }
    if (event == S_KEY) {
        if ((active_category == WEAPONS || active_category == ARMORS) && *active_item + 3 < category_items_count) {
            (*active_item) += 3;
        }
    }
    if (event == Z_KEY) {
        if ((active_category == WEAPONS && *active_item - 3 >= 0) ||
            (active_category == ARMORS && *active_item - 3 >= 0)) {
            (*active_item) -= 3;
        }
    }

    // choosing an item
    if (event == ENTER_KEY) {
        (*active_section)++;
    }
}

void handle_actions_input(event_t event, player_t *player, section_options_t *active_section, category_options_t active_category, unsigned short active_item, action_options_t *active_action) {
    // going back to previous section when pressing the left arrow on the left column
    if (event == Q_KEY) {
        if (active_category == MANA_POTIONS || active_category == HEALTH_POTIONS) {
            *active_section = CATEGORIES;
        } else {
            (*active_section)--;
        }
    }

    // moving through actions
    if (event == S_KEY && *active_action != THROW_AWAY) {
        (*active_action)++;
    }
    if (event == Z_KEY && *active_action != USE) {
        (*active_action)--;
    }

    // TODO : add actions for potions
    // choosing action
    if (event == ENTER_KEY) {
        if (*active_action == USE) {
            if (active_category == WEAPONS) {
                player->chosen_weapon = get_value_at_index(player->inventory->weapons_head, active_item);
                *active_section = ITEMS;
            }
            if (active_category == ARMORS) {
                player->chosen_armor = get_value_at_index(player->inventory->armors_head, active_item);
                *active_section = ITEMS;
            }
        }
        if (*active_action == THROW_AWAY) {
            if (active_category == WEAPONS) {
                delete_node(&(player->inventory->weapons_head), active_item);
                player->inventory->nb_weapons--;
                *active_section = ITEMS;
            }
            if (active_category == ARMORS) {
                delete_node(&(player->inventory->armors_head), active_item);
                player->inventory->nb_armors--;
                *active_section = ITEMS;
            }
        }
    }
}