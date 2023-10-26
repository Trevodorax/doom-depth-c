#include "fight_screen.h"
#include "fight_menu/fight_menu.h"
#include "fight_actions/fight_actions.h"
#include "fight_utils/fight_utils.h"
#include "display/display.h"
#include "fight_dimensions/fight_dimensions.h"
#include "../utils/utils.h"

int fight_screen(game_window_t * game_window, player_t * player, fight_t * fight) {
    if(game_window->ui_type == CLI) {
        cli_render_clear(game_window->matrix, (cli_char_t){' ', WHITE});
    }
    rect_t fight_zone;
    rect_t menu_zone;

    fight_context_t * fight_context = build_fight_context(fight, player);

    while (true) {
        update_fight_section_dimensions(game_window, &fight_zone, &menu_zone);
        menu_t * menu = build_nested_menu(fight_context);
        if (fight_context->player->is_defending) {
            fight_context->player->base_defense -= fight_context->player->defensive_spell->amount;
            fight_context->player->is_defending = false;
        }
        fight_action_t * selected_action = fight_menu(game_window, menu, fight_context, &fight_zone, &menu_zone, false);
        switch (selected_action->callback(fight_context, selected_action->params)) {
            case FA_QUIT:
                free_fight_context(fight_context);
                return MAP_SCREEN;
        }
        free_menu(menu);
    }
}

void monsters_turn(fight_context_t * fight_context){
    // TODO
    //  implement monsters turn
    //  don't forget to set player_turn to true at the end
}
