#include <time.h>
#include "fight_screen.h"
#include "fight_menu/fight_menu.h"
#include "fight_actions/fight_actions.h"
#include "fight_utils/fight_utils.h"

void update_section_dimensions(SDL_Window * window, SDL_Rect * fight_zone, SDL_Rect * menu_zone);

int fight_screen(game_window_t * game_window, player_t * player, fight_t * fight) {
    SDL_Rect fight_zone;
    SDL_Rect menu_zone;

    fight_context_t * fight_context = build_fight_context(fight,player);

    while (true) {
        menu_t * menu = build_nested_menu(fight_context);
        if(fight_context->player->is_defending){
            fight_context->player->base_defense -= fight_context->player->defensive_spell->amount;
            fight_context->player->is_defending = false;
        }
        fight_action_t * selected_action = fight_menu(game_window, menu, fight_context, &fight_zone, &menu_zone, false);
        switch (selected_action->callback(fight_context,selected_action->params)){
            case FA_QUIT:
                free(fight_context);
                return MAP_SCREEN;
        }
        free(menu);
    }
}

void monsters_turn(fight_context_t * fight_context){
    // TODO
    //  implement monsters turn
    //  don't forget to set player_turn to true at the end
}
