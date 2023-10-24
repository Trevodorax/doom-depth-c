#include "shop_screen.h"
#include "../utils/router.h"
#include "display/display.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "../inventory_screen/inventory_utils/inventory_utils.h"
#include "utils/shop_utils.h"
#include "../utils/items_management/types.h"

#define HEALTH_POTIONS_COST 10
#define MANA_POTIONS_COST 20

void handle_category_input(event_t event, bool *quit,
                           section_options *active_section, category_options *active_category, unsigned short *active_item);
void handle_shop_items_input(event_t event, unsigned short category_items_count,
                        section_options *active_section, category_options active_category,
                        unsigned short *active_item, confirm_options *active_confirmation);
void handle_confirm(event_t event, player_t *player, section_options *active_section, category_options active_category,
                    unsigned short active_item, confirm_options *active_option);
void handle_actions(player_t *player, category_options active_category, unsigned short active_item);

int shop_screen(game_window_t *game_window, player_t *player) {
    bool quit = false;
    section_options active_section = CATEGORIES;
    category_options active_category = ARMORS;
    confirm_options active_confirmation = YES;
    unsigned short active_item = 0;

    event_t event;
    while (true){
        delay(game_window->ui_type, 50);

        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }

        while (get_event(game_window->ui_type, &event)){
            if (event == QUIT) {
                return QUIT_GAME;
            }
            switch (active_section) {
                case CATEGORIES:
                    handle_category_input(event, &quit, &active_section, &active_category, &active_item);
                    if (quit) {
                        return QUIT_GAME;
                    }
                    break;

                case ITEMS: {
                    int category_items_count = get_items_count(active_category);
                    handle_shop_items_input(event, category_items_count, &active_section, active_category, &active_item, &active_confirmation);
                    break;
                }

                case CONFIRM:
                    handle_confirm(event, player, &active_section, active_category, active_item, &active_confirmation);

                default:
                    break;
            }
        }
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }
        if(display_shop(game_window, player, active_section, active_category, active_confirmation, active_item) == EXIT_FAILURE) {
            return QUIT_GAME;
        }
        render_present(game_window);
    }
}

void handle_category_input(
        event_t event,
        bool *quit,
        section_options *active_section,
        category_options *active_category,
        unsigned short *active_item) {
    switch (event) {
        case Z_KEY:
            if(*active_category != GO_BACK) {
                *active_category = GO_BACK;
            }
            break;

        case D_KEY:
        if(*active_category != GO_BACK && *active_category != HEALTH_POTIONS && *active_category != MANA_POTIONS) {
                (*active_category)++;
            }
            break;

        case S_KEY:
            if (*active_category == GO_BACK) {
                (*active_category)++;
            }
            break;

        case Q_KEY:
            if(*active_category != GO_BACK && *active_category != WEAPONS) {
                (*active_category)--;
            }
            break;

        case ENTER_KEY:
            if (*active_category == GO_BACK) {
                *quit = true;
                return;
            } else {
                (*active_section)++;
                *active_item = 0;
            }
            break;

        default:
            break;
    }
}

void handle_shop_items_input(event_t event, unsigned short category_items_count,
                        section_options *active_section, category_options active_category,
                        unsigned short *active_item, confirm_options *active_confirmation) {
    switch (event) {
        case Z_KEY:
            if (active_category == HEALTH_POTIONS || active_category == MANA_POTIONS || *active_item < 3) {
                *active_section = CATEGORIES;
            } else {
                *active_item -= 3;
            }
            break;

        case D_KEY:
            if (*active_item % 3 != 2 && *active_item < category_items_count - 1) {
                (*active_item)++;
            }
            break;

        case S_KEY:
            if ((active_category == WEAPONS || active_category == ARMORS) && *active_item + 3 < category_items_count) {
                (*active_item) += 3;
            }
            break;

        case Q_KEY:
            if (*active_item % 3 != 0) {
                (*active_item)--;
            }
            break;

        case ENTER_KEY:
            *active_section = CONFIRM;
            *active_confirmation = YES;
            break;

        default:
            break;

    }
}

void handle_confirm(event_t event, player_t *player, section_options *active_section, category_options active_category,
                    unsigned short active_item, confirm_options *active_option) {
    switch (event) {
        case Z_KEY:
            if (*active_option == NO) {
                *active_option = YES;
            }
            break;

        case S_KEY:
            if (*active_option == YES) {
                *active_option = NO;
            }
            break;

        case ENTER_KEY:
            if (*active_option == NO) {
                *active_section = ITEMS;
            } else {
                handle_actions(player, active_category, active_item);
            }

        case D_KEY:
        case Q_KEY:
        default:
            return;
    }
}

void handle_actions(player_t *player, category_options active_category, unsigned short active_item) {
    switch (active_category) {
        case WEAPONS: {
            array_node_t *weapons = get_weapons();
            weapon_t *weapon_to_buy = get_value_at_index(weapons, active_item);
            if (player->gold >= weapon_to_buy->cost && !is_full(player->inventory)) {
                push(&player->inventory->weapons_head, weapon_to_buy, sizeof(weapon_t));
                player->inventory->nb_weapons++;
                player->gold -= weapon_to_buy->cost;
            }
            break;
        }

        case ARMORS: {
            array_node_t *armors = get_armors();
            armor_t *armor_to_buy = get_value_at_index(armors, active_item);
            if (player->gold >= armor_to_buy->cost && !is_full(player->inventory)) {
                push(&player->inventory->armors_head, armor_to_buy, sizeof(armor_t));
                player->inventory->nb_armors++;
                player->gold -= armor_to_buy->cost;
            }
            break;
        }

        case HEALTH_POTIONS:
        case MANA_POTIONS: {
            int amount_needed = (active_item == HEALTH) ? HEALTH_POTIONS_COST : MANA_POTIONS_COST;
            if (player->gold >= amount_needed && !is_full(player->inventory)) {
                if (active_item == HEALTH) {
                    player->inventory->nb_health_potions++;
                    player->gold -= HEALTH_POTIONS_COST;
                }
                if (active_item == MANA) {
                    player->inventory->nb_mana_potions++;
                    player->gold -= MANA_POTIONS_COST;
                }
            }
            break;
        }

        case GO_BACK:
        default:
            return;
    }
}