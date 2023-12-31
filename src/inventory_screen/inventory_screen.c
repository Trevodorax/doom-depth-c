#include "inventory_screen.h"
#include "../utils/router.h"
#include "display/display.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "../logs/log.h"
#include "../help_screen/help_screen.h"
#include "../stats_screen/stats_screen.h"

void handle_categories_input(event_t event, bool *quit, section_options_t *active_section, category_options_t *active_category, unsigned short *active_item);
void handle_items_input(event_t event, section_options_t *active_section, category_options_t active_category,
                        unsigned short *active_item, action_options_t *active_action,
                        unsigned short category_items_count, player_t *player);
void handle_actions_input(event_t event, player_t *player, section_options_t *active_section, category_options_t active_category, unsigned short active_item, action_options_t *active_action);

int inventory_screen(game_window_t *game_window, player_t *player) {
    if (!game_window) {
        global_logger->error("Cannot display inventory : no game window\n");
        return EXIT_FAILURE;
    }
    if (!player) {
        global_logger->error("Cannot display inventory : no player\n");
        return EXIT_FAILURE;
    }

    if(game_window->ui_type == CLI) {
        resize_cli_matrix_to_window(game_window->matrix, (cli_char_t){' ', game_window->cli_color_palette->background});
    }

    event_t event;
    bool quit = false;
    section_options_t active_section = CATEGORIES;
    category_options_t active_category = WEAPONS;
    action_options_t active_action = USE;
    unsigned short active_item = 0;
    while (!quit){
        delay(game_window->ui_type, 50);
        set_cli_raw_mode(true);
        while (get_event(game_window->ui_type, &event)) {
            switch (event) {
                case m_KEY:
                    stats_screen(game_window, player);
                    break;
                case h_KEY:
                    help_screen(game_window);
                    break;
                case z_KEY:
                case q_KEY:
                case s_KEY:
                case d_KEY:
                case ENTER_KEY:
                    switch (active_section){
                        case CATEGORIES:
                            handle_categories_input(event, &quit, &active_section, &active_category, &active_item);
                            if (quit) {
                                return EXIT_SUCCESS;
                            }
                            break;

                        case ITEMS:
                            if (active_category == WEAPONS) {
                                handle_items_input(event, &active_section, active_category, &active_item, &active_action,
                                                   player->inventory->nb_weapons, player);
                            } else if (active_category == ARMORS) {
                                handle_items_input(event, &active_section, active_category, &active_item, &active_action,
                                                   player->inventory->nb_armors, player);
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
        // set_cli_raw_mode(false);
        display_inventory(game_window, player, active_section, active_category, active_action, active_item);
        render_present(game_window);
    }
    return EXIT_SUCCESS;
}

void handle_categories_input(event_t event, bool *quit, section_options_t *active_section, category_options_t *active_category, unsigned short *active_item) {
    // navigating through the categories
    if (event == s_KEY && *active_category != MANA_POTIONS) {
    (*active_category)++;
    *active_item = 0;
    }
    if (event == z_KEY && *active_category != GO_BACK) {
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

void handle_items_input(event_t event, section_options_t *active_section, category_options_t active_category,
                        unsigned short *active_item, action_options_t *active_action,
                        unsigned short category_items_count, player_t *player) {
    // going back to categories when pressing the left arrow on the left column
    if (event == q_KEY) {
        if (*active_item % 3 == 0) {
            (*active_section)--;
        } else {
            (*active_item)--;
        }
    }

    // moving in the items grid
    if (event == d_KEY) {
        if (*active_item % 3 != 2 && *active_item < category_items_count - 1) {
            (*active_item)++;
        }
    }
    if (event == s_KEY) {
        if ((active_category == WEAPONS || active_category == ARMORS) && *active_item + 3 < category_items_count) {
            (*active_item) += 3;
        }
    }
    if (event == z_KEY) {
        if ((active_category == WEAPONS && *active_item - 3 >= 0) ||
            (active_category == ARMORS && *active_item - 3 >= 0)) {
            (*active_item) -= 3;
        }
    }

    // choosing an item
    if (event == ENTER_KEY) {
        if ((active_category == ARMORS && get_value_at_index(player->inventory->armors_head, *active_item) == player->chosen_armor) ||
            (active_category == WEAPONS && get_value_at_index(player->inventory->weapons_head, *active_item) == player->chosen_weapon)) {
            *active_action = UNEQUIP;
        } else {
            *active_action = USE;
        }
        (*active_section)++;
    }
}

void handle_actions_input(event_t event, player_t * player, section_options_t * active_section,
                          category_options_t active_category, unsigned short active_item,
                          action_options_t * active_action) {
    bool can_be_used;
    bool can_be_unequipped;
    if ((active_category == ARMORS && get_value_at_index(player->inventory->armors_head, active_item) == player->chosen_armor) ||
        (active_category == WEAPONS && get_value_at_index(player->inventory->weapons_head, active_item) == player->chosen_weapon)) {
        can_be_used = false;
        can_be_unequipped = true;
    } else {
        can_be_used = true;
        can_be_unequipped = false;
    }

    // going back to previous section when pressing the left arrow on the left column
    if (event == q_KEY) {
        if (active_category == MANA_POTIONS || active_category == HEALTH_POTIONS) {
            *active_section = CATEGORIES;
        } else {
            (*active_section)--;
        }
    }

    // moving through actions
    if (event == s_KEY && *active_action != THROW_AWAY) {
        if ((*active_action == USE && can_be_unequipped) || *active_action == UNEQUIP) {
            (*active_action)++;
        } else {
            (*active_action) = THROW_AWAY;
        }
    }
    if (event == z_KEY && *active_action != (can_be_used ? USE : UNEQUIP)) {
        if (*active_action == THROW_AWAY && can_be_unequipped) {
            (*active_action)--;
        } else {
            (*active_action) = USE;
        }
    }

    // choosing action
    if (event == ENTER_KEY) {
        switch (*active_action) {
            case USE:
                if (active_category == WEAPONS) {
                    player->chosen_weapon = get_value_at_index(player->inventory->weapons_head, active_item);
                    player->chosen_weapon->chosen = true;
                    *active_section = ITEMS;
                }
                if (active_category == ARMORS) {
                    player->chosen_armor = get_value_at_index(player->inventory->armors_head, active_item);
                    player->chosen_armor->chosen = true;
                    *active_section = ITEMS;
                }
                if (active_category == HEALTH_POTIONS) {
                    heal_player(player, 20);
                    if (player->inventory->nb_health_potions > 0) {
                        (player->inventory->nb_health_potions)--;
                    }
                }
                if (active_category == MANA_POTIONS) {
                    heal_mana(player, 20);
                    if (player->inventory->nb_mana_potions > 0) {
                        (player->inventory->nb_mana_potions)--;
                    }
                }
                break;

            case UNEQUIP:
                if (active_category == WEAPONS) {
                    player->chosen_weapon->chosen = false;
                    player->chosen_weapon = NULL;
                    *active_section = ITEMS;
                }
                if (active_category == ARMORS) {
                    player->chosen_armor->chosen = false;
                    player->chosen_armor = NULL;
                    *active_section = ITEMS;
                }
                break;

            case THROW_AWAY:
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
                if (active_category == HEALTH_POTIONS && player->inventory->nb_health_potions > 0) {
                    (player->inventory->nb_health_potions)--;
                }
                if (active_category == MANA_POTIONS && player->inventory->nb_mana_potions > 0) {
                    (player->inventory->nb_mana_potions)--;
                }
                break;
        }
    }
}