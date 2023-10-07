#include "fight_screen.h"
#include "fight_menu/fight_menu.h"

void update_section_dimensions(SDL_Window * window, SDL_Rect * fight_zone, SDL_Rect * menu_zone);

int fight_screen(game_window_t * game_window, player_t * player, fight_t * fight) {
    SDL_Rect fight_zone;
    SDL_Rect menu_zone;

    menu_t * menu = build_nested_menu();
    fight_context_t * fight_context = malloc(sizeof(fight_context_t));

    while (true) {
        fight_action_t * selected_action = fight_menu(game_window, menu, fight_context, &fight_zone, &menu_zone, false);
        if(selected_action->callback(fight_context, selected_action->params) == -1) {
            return MAP_SCREEN;
        }
    }
}
