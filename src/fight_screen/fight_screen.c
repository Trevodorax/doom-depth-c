#include "fight_screen.h"
#include "fight_menu/fight_menu.h"
#include "fight_actions/fight_actions.h"
#include "fight_utils/fight_utils.h"
#include "display/display.h"
#include "fight_dimensions/fight_dimensions.h"
#include "../utils/utils.h"
#include "../level_up_screen/level_up_screen.h"

void monsters_turn(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone);
void monster_turn(game_window_t * game_window, player_t * player, monster_t * monster, fight_context_t * fight_context, rect_t fight_zone);

router_t fight_screen(game_window_t *game_window, player_t *player, stage_t *stage, fight_context_t * previous_fight_context) {
    if (game_window->ui_type == CLI) {
        cli_render_clear(game_window->matrix, (cli_char_t){' ', game_window->cli_color_palette->background});
    }
    rect_t fight_zone;
    rect_t menu_zone;

    fight_context_t * fight_context = NULL;
    if (previous_fight_context) {
        fight_context = previous_fight_context;
    } else {
        fight_context = build_fight_context(stage->fight, player);
    }

    player_state_checkpoint(player, true);

    while (true) {
        update_fight_section_dimensions(game_window, &fight_zone, &menu_zone);
        menu_t * menu = build_nested_menu(fight_context);
        if (fight_context->player_turn) {
            fight_action_t * selected_action = fight_menu(game_window, menu, fight_context, &fight_zone, &menu_zone,
                                                          false, stage);
            if (selected_action == NULL) {
                return QUIT_GAME;
            }
            switch (selected_action->callback(fight_context, selected_action->params)) {
                case FA_QUIT: {
                    if(get_size(fight_context->monsters) == 0) {
                        build_notification_formatted(fight_context, "Player wins %d gold from fight !", fight_context->treasure->coins);
                        display_fight(game_window, fight_context, fight_zone);
                        render_present(game_window);
                        delay(game_window->ui_type, 2000);
                        give_treasure_to_player(fight_context->treasure, fight_context->player);

                        // level up
                        if (check_level_up(player)) {
                            level_up_screen(game_window, fight_context->player);
                        };
                    }

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
            if(player->hp == 0) {
                (player->stats->nb_deaths)++;
                stage->fight_context = NULL;
                return GAME_OVER;
            }

            fight_context->player_turn = true;
            player->action_points = player->max_action_points;
            if (fight_context->player->is_defending) {
                fight_context->player->base_defense -= fight_context->player->defensive_spell->amount;
                fight_context->player->is_defending = false;
            }
            heal_mana(fight_context->player, 10);
        }

        free_menu(menu);
    }
}

void monsters_turn(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone) {
    array_node_t * current_monster_node = fight_context->monsters;

    while(current_monster_node) {
        monster_t * current_monster = void_to_monster(current_monster_node->value);

        monster_turn(game_window, fight_context->player, current_monster, fight_context, fight_zone);
        if (fight_context->player->hp == 0) {
            break;
        }

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
