#include <time.h>
#include "fight_utils.h"

unsigned int player_attack(player_t * player, monster_t * target) {
    weapon_t * weapon = player->chosen_weapon;
    unsigned int damages = 0;
    if(weapon != NULL){
        unsigned int random = weapon->min_attack + (rand() % (weapon->max_attack - weapon->min_attack + 1));
        damages = (random + player->base_attack) - target->defense;
    } else {
        damages = player->base_attack - target->defense;
    }

    if(damages > target->hp){
        damages = target->hp;
    }
    target->hp -= damages;

    give_exp(player,damages);

    return damages;
}

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

// FIXME : to remove when we can search on monsters by name
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

void debug_action_points(player_t *player){
    printf("\nAction points : %d/%d", player->action_points, player->max_action_points);
}

bool check_and_remove_action_points(player_t *player, unsigned int amount) {
    if (player->action_points < amount) {
        debug_action_points(player);
        return false;
    } else {
        player->action_points -= amount;
        debug_action_points(player);
        return true;
    }
}

fight_context_t * build_fight_context(fight_t * fight, player_t * player) {
    srand(time(NULL));
    fight_context_t * fight_context = malloc(sizeof(fight_context_t));
    fight_context->player = player;
    fight_context->monsters = NULL;

    int enemies_size = fight->enemies_size;

    unsigned int number_of_monsters = (rand() % (fight->max_nb_enemies - fight->min_nb_enemies + 1)) + fight->min_nb_enemies;

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
        // FIXME : add monster chosen on fight context, waiting so we can search on monsters by name
        append(&fight_context->monsters,get_monster_with_name(fight->enemies_list[index]),sizeof(monster_t));
    }

    fight_context->notification_message = NULL;
    fight_context->player_turn = true;
    return fight_context;
}
