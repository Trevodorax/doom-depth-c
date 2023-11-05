#include "fight_screen.h"
#include "fight_menu/fight_menu.h"
#include "fight_actions/fight_actions.h"
#include "fight_utils/fight_utils.h"
#include "display/display.h"
#include "fight_dimensions/fight_dimensions.h"
#include "../utils/utils.h"

void monsters_turn(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone);
void monster_turn(game_window_t * game_window, player_t * player, monster_t * monster, fight_context_t * fight_context, rect_t fight_zone);

router_t fight_screen(game_window_t * game_window, player_t * player, fight_t * fight) {
    if(game_window->ui_type == CLI) {
        cli_render_clear(game_window->matrix, (cli_char_t){' ', WHITE});
    }
    rect_t fight_zone;
    rect_t menu_zone;

    fight_context_t * fight_context = build_fight_context(fight, player);

    while (true) {
        update_fight_section_dimensions(game_window, &fight_zone, &menu_zone);
        menu_t * menu = build_nested_menu(fight_context);
        if (fight_context->player_turn) {
            fight_action_t * selected_action = fight_menu(game_window, menu, fight_context, &fight_zone, &menu_zone, false);
            switch (selected_action->callback(fight_context, selected_action->params)) {
                case FA_QUIT: {
                    free_fight_context(fight_context);
                    player->action_points = player->max_action_points;
                    return MAP_SCREEN;
                }
                case FA_END_TURN: {
                    fight_context->player_turn = false;
                    player->action_points = player->max_action_points;
                    break;
                }
                default: {
                    if(fight_context->player->action_points == 0) {
                        fight_context->player_turn = false;
                    }
                }
            }
        } else {
            monsters_turn(game_window, fight_context, fight_zone);
            fight_context->player_turn = true;
            player->action_points = player->max_action_points;
        }

        free_menu(menu);
    }
}

void monsters_turn(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone) {
    array_node_t * current_monster_node = fight_context->monsters;

    while(current_monster_node) {
        monster_t * current_monster = void_to_monster(current_monster_node->value);

        monster_turn(game_window, fight_context->player, current_monster, fight_context, fight_zone);

        current_monster_node = current_monster_node->next;
    }

    strcpy(fight_context->notification_message, "");
}

void monster_turn(game_window_t * game_window, player_t * player, monster_t * monster, fight_context_t * fight_context, rect_t fight_zone) {
    if(!monster || monster->hp == 0) {
        return;
    }

    unsigned damages = monster_attack(monster, player);
    build_notification_formatted(fight_context, "%s deals %zu damages to %s !", monster->name, damages, player->name);
    display_fight(game_window, fight_context, fight_zone);
    render_present(game_window);
    delay(game_window->ui_type, 1000);

}
