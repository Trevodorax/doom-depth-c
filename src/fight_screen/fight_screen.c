#include <time.h>
#include "fight_screen.h"
#include "fight_menu/fight_menu.h"
#include "fight_actions/fight_actions.h"

void update_section_dimensions(SDL_Window * window, SDL_Rect * fight_zone, SDL_Rect * menu_zone);

void build_notification(fight_context_t * fight_context, char * message) {
    fight_context->notification_message = malloc(sizeof(char) * strlen(message)+1);
    strcpy(fight_context->notification_message, message);
    printf("\n%s",fight_context->notification_message);
}

void build_notification_formatted(fight_context_t * fight_context, char * message, ...) {
    va_list args;
    va_start(args, message);
    fight_context->notification_message = malloc(sizeof(char) * strlen(message)+1001);
    vsprintf(fight_context->notification_message, message, args);
    va_end(args);
    printf("\n%s",fight_context->notification_message);
}

int find_index(int rand_num, const int probs[], int size) {
    for (int i = 0; i < size; i++) {
        if (rand_num < probs[i]) {
            return i;
        }
    }
    return -1;  // This should never happen if input data is correct
}

void * get_monster_with_name(char * name){
    if(strcmp(name,"bat") == 0){
        monster_t * bat = malloc(sizeof(monster_t));
        bat->name = malloc(sizeof(char)*4);
        strcpy(bat->name,"Bat");
        bat->type = 0;
        bat->attack = 2;
        bat->defense = 1;
        bat->hp = 40;
        bat->hp_max = 40;
        bat->id = 1;
        return bat;
    } else if(strcmp(name,"goblin") == 0){
        monster_t * goblin = malloc(sizeof(monster_t));
        goblin->name = malloc(sizeof(char)*7);
        strcpy(goblin->name,"Goblin");
        goblin->type = 0;
        goblin->attack = 2;
        goblin->defense = 1;
        goblin->hp = 40;
        goblin->hp_max = 40;
        goblin->id = 1;
        return goblin;
    } else if(strcmp(name,"troll") == 0){
        monster_t * troll = malloc(sizeof(monster_t));
        troll->name = malloc(sizeof(char)*6);
        strcpy(troll->name,"Troll");
        troll->type = 0;
        troll->attack = 2;
        troll->defense = 1;
        troll->hp = 40;
        troll->hp_max = 40;
        troll->id = 1;
        return troll;
    }

}

fight_context_t * build_fight_context(fight_t * fight, player_t * player){
    srand(time(NULL));
    fight_context_t * fight_context = malloc(sizeof(fight_context_t));
    fight_context->player = player;
    fight_context->monsters = NULL;

    int enemies_size = fight->enemies_size;

    int number_of_monsters = (rand() % (fight->max_nb_enemies - fight->min_nb_enemies + 1)) + fight->min_nb_enemies;

    int * probs = malloc(enemies_size * sizeof(int));
    if(probs == NULL) {
        exit(EXIT_FAILURE);
    }
    probs[0] = fight->enemies_chances_to_appear[0];
    for (int i = 1; i < fight->enemies_size; i++) {
        probs[i] = probs[i - 1] + fight->enemies_chances_to_appear[i];
    }

    for(int i = 0; i < number_of_monsters; i++){
        int random_number = rand() % 100;
        int index = find_index(random_number,probs,fight->enemies_size);
        // TODO : add monster chosen on fight context, waiting so we can search on monsters by name
        append(&fight_context->monsters,get_monster_with_name(fight->enemies_list[index]),sizeof(monster_t));
    }

    fight_context->notification_message = NULL;
    fight_context->player_turn = true;
    return fight_context;
}

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
