#include "fight_menu.h"
#include <stdlib.h>
#include <stdio.h>
#include "../fight_actions/fight_actions.h"
#include "../fight_dimensions/fight_dimensions.h"
#include "../../event/event.h"
#include "display/display.h"
#include "../../utils/utils.h"
#include "../display/display.h"
#include "../fight_utils/fight_utils.h"
#include "../../map_screen/map/map.h"
#include "../../inventory_screen/inventory_screen.h"
#include "../../help_screen/help_screen.h"
#include "../../confirm_quit_screen/confirm_quit_screen.h"
#include "../../stats_screen/stats_screen.h"
#include "../../game_stats_screen/game_stats_screen.h"

menu_t* create_menu(int nb_options, const char * title, const char * image_path, int (*callback)(fight_context_t *, void * custom_params), void * custom_params) {
    menu_t *new_menu = malloc(sizeof(menu_t));
    new_menu->nb_options = nb_options;
    new_menu->options = malloc(sizeof(menu_t *) * nb_options);
    new_menu->title = strdup(title);

    if (image_path) {
        new_menu->image_path = malloc(strlen(image_path));
        strcpy(new_menu->image_path, image_path);
    } else {
        new_menu->image_path = malloc(1);
        new_menu->image_path[0] = '\0';
    }

    new_menu->action = malloc(sizeof(fight_action_t));
    new_menu->action->callback = callback;
    new_menu->action->params = custom_params;

    return new_menu;
}

menu_t* build_nested_menu(fight_context_t * fight_context) {
    int monsters_size = get_size(fight_context->monsters);

    menu_t *root_menu = create_menu(3, "Root Menu", NULL, NULL, NULL);

    menu_t *sub_menu_weapon = create_menu(monsters_size, "Weapon", NULL, NULL, NULL);
    menu_t *sub_menu_spells = create_menu(3, "Spells", NULL, NULL, NULL);

    sub_menu_spells->options[0] = create_menu(monsters_size, "Attack Spell", NULL, NULL, NULL);
    sub_menu_spells->options[1] = create_menu(0, "Defend Spell", NULL, defend_spell, NULL);
    sub_menu_spells->options[2] = create_menu(0, "Heal Spell", NULL, heal_spell, NULL);

    array_node_t * current_monster = fight_context->monsters;
    int i = 0;
    while(current_monster != NULL){
        monster_t * monster = (monster_t*)current_monster->value;
        if (!monster->name) {
            current_monster = current_monster->next;
            continue;
        }
        char * name_and_hp = malloc(sizeof(char)*(strlen(monster->name)+10));
        sprintf(name_and_hp,"%s (%d)",monster->name,monster->hp);
        sub_menu_weapon->options[i] = create_menu(0, name_and_hp, NULL, attack_weapon, monster);
        sub_menu_spells->options[0]->options[i++] = create_menu(0, name_and_hp, NULL, attack_spell, monster);
        current_monster = current_monster->next;
    }

    menu_t *sub_menu_others = create_menu(2, "Others", NULL, NULL, NULL);

    sub_menu_others->options[0] = create_menu(2, "Bag", NULL, NULL, NULL);
    sub_menu_others->options[0]->options[0] = create_menu(0, "Heal Potion", NULL, heal_potion, NULL);
    sub_menu_others->options[0]->options[1] = create_menu(0, "Mana Potion", NULL, mana_potion, NULL);

    sub_menu_others->options[1] = create_menu(0, "End Turn", NULL, end_turn, NULL);

    root_menu->options[0] = sub_menu_weapon;
    root_menu->options[1] = sub_menu_spells;
    root_menu->options[2] = sub_menu_others;
    return root_menu;
}

void free_menu(menu_t * menu) {
    if (!menu)
        return;

    for (int i = 0; i < menu->nb_options; i++) {
        free_menu(menu->options[i]);
    }

    free(menu->title);
    free(menu->image_path);
    free(menu->options);
    free(menu);
}

fight_action_t * fight_menu(game_window_t *game_window, menu_t *menu, fight_context_t *fight_context, rect_t *fight_zone,
                           rect_t *menu_zone, bool is_nested, stage_t * stage) {
    int selected_item_index = 0;
    event_t event;
    while (true){
        set_cli_raw_mode(true);
        while (get_event(game_window->ui_type, &event)) {
            switch(event) {
                case m_KEY:
                    stats_screen(game_window, fight_context->player);
                    break;
                case p_KEY:
                    game_stats_screen(game_window, fight_context->player);
                    break;
                case h_KEY:
                    help_screen(game_window);
                    break;
                case z_KEY:
                    selected_item_index = handle_fight_menu_movement(selected_item_index, menu->nb_options, NORTH);
                    break;
                case q_KEY:
                    selected_item_index = handle_fight_menu_movement(selected_item_index, menu->nb_options, WEST);
                    break;
                case s_KEY:
                    selected_item_index = handle_fight_menu_movement(selected_item_index, menu->nb_options, SOUTH);
                    break;
                case d_KEY:
                    selected_item_index = handle_fight_menu_movement(selected_item_index, menu->nb_options, EAST);
                    break;
                case i_KEY:
                    inventory_screen(game_window, fight_context->player);
                    break;
                case Q_KEY:
                case QUIT:
                    stage->fight_context = fight_context;
                    if (confirm_quit_screen(game_window) == QUIT_GAME) {
                        return NULL;
                    }
                    break;
                case ENTER_KEY:
                    if(!fight_context->player_turn){
                        break;
                    }
                    menu_t * clicked_menu = menu->options[selected_item_index];
                    if (clicked_menu->nb_options > 0) {
                        fight_action_t * selected_action = fight_menu(game_window, clicked_menu, fight_context,
                                                                      fight_zone, menu_zone, true, NULL);
                        if (!selected_action || !selected_action->callback) {
                            break;
                        } else {
                            return selected_action;
                        }
                    }
                    if (clicked_menu->action->callback) {
                        return clicked_menu->action;
                    }
                    // if there is no submenu AND no action, then there is a problem and nothing happens
                    return NULL;
                case ESCAPE_KEY:
                    if (is_nested) {
                        return NULL;
                    }
                    break;
                default:
                    break;
            }
        }
        set_cli_raw_mode(false);
        update_fight_section_dimensions(game_window, fight_zone, menu_zone);
        display_menu(game_window, menu, *menu_zone, selected_item_index, fight_context->player_turn);
        display_fight(game_window, fight_context, *fight_zone);
        render_present(game_window);
        delay(game_window->ui_type, 50);
    }
}

int handle_fight_menu_movement(int selected_item_index, int nb_items, orientation_t direction) {
    int nb_rows = (int)sqrt(nb_items);
    int nb_cols = (nb_items + nb_rows - 1) / nb_rows;

    switch (direction) {
        case NORTH:
            selected_item_index -= nb_cols;
            break;
        case EAST:
            selected_item_index ++;
            break;
        case SOUTH:
            selected_item_index += nb_cols;
            break;
        case WEST:
            selected_item_index--;
            break;
        default:
            return selected_item_index;
    }

    if (selected_item_index < 0) {
        return 0;
    }

    if (selected_item_index >= nb_items) {
        return nb_items - 1;
    }

    return selected_item_index;
}
