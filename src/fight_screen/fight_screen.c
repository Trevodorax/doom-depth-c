#include <time.h>
#include "fight_screen.h"
#include "fight_menu/fight_menu.h"

void update_section_dimensions(SDL_Window * window, SDL_Rect * fight_zone, SDL_Rect * menu_zone);

int find_index(int rand_num, const int probs[], int size) {
    for (int i = 0; i < size; i++) {
        if (rand_num < probs[i]) {
            return i;
        }
    }
    return -1;  // This should never happen if input data is correct
}

fight_context_t * build_fight_context(fight_t * fight, player_t * player){
    srand(time(NULL));
    fight_context_t * fight_context = malloc(sizeof(fight_context_t));
    fight_context->player = player;

    int enemies_size = fight->enemies_size;

    int number_of_monsters = (rand() % (fight->max_nb_enemies - fight->min_nb_enemies + 1)) + fight->min_nb_enemies;

    int probs[enemies_size];
    probs[0] = fight->enemies_chances_to_appear[0];
    for (int i = 1; i < fight->enemies_size; i++) {
        probs[i] = probs[i - 1] + fight->enemies_chances_to_appear[i];
    }

    for(int i = 0; i < number_of_monsters; i++){
        int random_number = rand() % 100;
        int index = find_index(random_number,probs,fight->enemies_size);
        // TODO : add monster chosen on fight context, waiting so we can search on monsters by name
        // append(&fight_context->monsters,get_monster_with_name(fight->enemies_list[index]),sizeof(monster_t));
    }
    return fight_context;
}

int fight_screen(game_window_t * game_window, player_t * player, fight_t * fight) {


    SDL_Rect fight_zone;
    SDL_Rect menu_zone;

    menu_t * menu = build_nested_menu();
    fight_context_t * fight_context = build_fight_context(fight,player);

    fight_context->player = player;


    while (true) {
        fight_action_t * selected_action = fight_menu(game_window, menu, fight_context, &fight_zone, &menu_zone, false);
        if(selected_action->callback(fight_context, selected_action->params) == -1) {
			free(fight_context);
            return MAP_SCREEN;
        }
    }
}
