#include <time.h>
#include "fight_utils.h"
#include "../../entities/entities.h"

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
    player->stats->damages_dealt += damages;

    return damages;
}

unsigned int monster_attack(monster_t * monster, player_t * target) {
    unsigned int armor = target->chosen_armor ?  target->chosen_armor->amount : 0;
    unsigned int damages = 0;
    if(monster->attack > target->base_defense+armor){
        damages = monster->attack - (target->base_defense+armor);
    }

    if(damages > target->hp){
        damages = target->hp;
    }
    target->hp -= damages;

    global_logger->info("\nMonster attack : %d damages", damages);
    global_logger->info("\nPlayer hp : %d/%d", target->hp, target->hp_max);

    return damages;
}

void build_notification(fight_context_t * fight_context, char * message) {
    if(fight_context->notification_message == NULL) {
        fight_context->notification_message = malloc(sizeof(char) * strlen(message) + 1001);
    }

    strcpy(fight_context->notification_message, message);
    global_logger->info("%s", fight_context->notification_message);
}

void build_notification_formatted(fight_context_t * fight_context, char * message, ...) {
    va_list args;
    va_start(args, message);
    if(fight_context->notification_message == NULL) {
        fight_context->notification_message = malloc(sizeof(char) * strlen(message) + 1001);
    }

    vsprintf(fight_context->notification_message, message, args);
    va_end(args);
    global_logger->info( "%s", fight_context->notification_message);
}

int find_index(int rand_num, const int probs[], int size) {
    for (int i = 0; i < size; i++) {
        if (rand_num < probs[i]) {
            return i;
        }
    }
    return -1;  // This should never happen if input data is correct
}

void free_fight_context(fight_context_t * fight_context) {
    free(fight_context->notification_message);
    free_list(&fight_context->monsters);
    free(fight_context);
}

void debug_action_points(player_t *player){
    global_logger->info("\nAction points : %d/%d", player->action_points, player->max_action_points);
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

bool check_and_remove_mana(player_t * player, unsigned int amount) {
    if (player->mana < amount) {
        return false;
    } else {
        player->mana -= amount;
        return true;
    }
}

treasure_t * get_treasure_from_fight_context(fight_context_t * fight_context);

fight_context_t * build_fight_context(fight_t * fight, player_t * player) {
    srand(time(NULL));
    fight_context_t * fight_context = malloc(sizeof(fight_context_t));
    fight_context->player = player;
    fight_context->monsters = NULL;
    global_logger->info("Building Fight Context");

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

    while(get_size(fight_context->monsters) < number_of_monsters) {
        int random_number = rand() % 100;
        int index = find_index(random_number,probs,fight->enemies_size);
        append(&fight_context->monsters,get_monster_by_name(fight->enemies_list[index]),sizeof(monster_t));
    }

    fight_context->notification_message = NULL;
    fight_context->player_turn = true;

    // loot
    fight_context->treasure = get_treasure_from_fight_context(fight_context);
    return fight_context;
}

treasure_t * get_treasure_from_fight_context(fight_context_t * fight_context) {
    // safeguards
    if (!fight_context) {
        return NULL;
    }

    // calculate number of coins
    int coins = 0;
    array_node_t *current = fight_context->monsters;
    while (current != NULL) {
        monster_t *monster = void_to_monster(current->value);
        if (monster) {
            coins += (int)(monster->hp_max + monster->attack) / 2;
        }
        current = current->next;
    }

    // create and return treasure
    treasure_t *treasure = malloc(sizeof(treasure_t));
    if (!treasure) return NULL;
    treasure->coins = coins;
    treasure->mana_potions = get_size(fight_context->monsters);
    treasure->armor = NULL;
    treasure->weapon = NULL;
    return treasure;
}

json_t * fight_context_to_json(fight_context_t * fight_context) {
    if (!fight_context) {
        global_logger->error("\nfight_context_to_json error: wrong parameters");
        return NULL;
    }

    json_t * fight_context_json = calloc(1, sizeof(json_t));
    fight_context_json->type = 'o';

    // monsters
    if (fight_context->monsters) {
        int nb_monsters = get_size(fight_context->monsters);

        json_t * json_monsters = malloc(sizeof(json_t));
        json_monsters->type = 'a';
        json_monsters->nb_elements = nb_monsters;
        json_monsters->values = malloc(nb_monsters * sizeof(json_t));

        array_node_t * current_monster = fight_context->monsters;
        int monster_index = 0;
        while (current_monster != NULL) {
            monster_t * monster = void_to_monster(current_monster->value);
            json_t * monster_json = monster_to_json(monster);

            json_monsters->values[monster_index] = *monster_json;

            monster_index++;
            current_monster = current_monster->next;
        }
        add_key_value_to_object(&fight_context_json, "monsters", json_monsters);
    }

    // notification_message
    if (fight_context->notification_message) {
        json_t * json_notification = malloc(sizeof(json_t));
        json_notification->type = 's';
        json_notification->string = strdup(fight_context->notification_message);
        add_key_value_to_object(&fight_context_json, "notification_message", json_notification);
    }

    // player_turn
    json_t * json_player_turn = malloc(sizeof(json_t));
    json_player_turn->type = 'n';
    json_player_turn->number = fight_context->player_turn ? 1 : 0;
    add_key_value_to_object(&fight_context_json, "player_turn", json_player_turn);

    // treasure
    if (fight_context->treasure) {
        json_t * json_treasure = malloc(sizeof(json_t));
        json_treasure->type = 'o';
        json_treasure->nb_elements = 0;
        json_treasure->keys = NULL;
        json_treasure->values = NULL;
        add_treasure_to_json_object(json_treasure, fight_context->treasure);
        add_key_value_to_object(&fight_context_json, "treasure", json_treasure);
    }

    return fight_context_json;
}

fight_context_t * json_to_fight_context(json_t * object) {
    if (!object || object->type != 'o') {
        global_logger->error("\njson_to_fight_context error: wrong parameters");
        return NULL;
    }

    fight_context_t * context = malloc(sizeof(fight_context_t));
    if (!context) {
        global_logger->error("\njson_to_fight_context error: memory allocation failed");
        return NULL;
    }

    // monsters
    json_t * json_monsters = get_object_at_key(object, "monsters");
    if (json_monsters) {
        if (json_monsters->type == 'a') {
            size_t nb_monsters = json_monsters->nb_elements;
            array_node_t *head = NULL;

            for (int i = 0; i < nb_monsters; ++i) {
                json_t * monster_json = &json_monsters->values[i];
                monster_t * monster = json_to_monster(monster_json);

                append(&head, monster, sizeof(monster_t));
            }

            context->monsters = head;
        }
    }

    // notification_message
    json_t * json_message = get_object_at_key(object, "notification_message");
    if (json_message && json_message->type == 's') {
        context->notification_message = strdup(json_message->string);
    }

    // player_turn
    json_t * json_player_turn = get_object_at_key(object, "player_turn");
    if (json_player_turn && json_player_turn->type == 'n') {
        context->player_turn = json_player_turn->number != 0;
    }

    // treasure
    json_t * json_treasure = get_object_at_key(object, "treasure");
    if (json_treasure) {
        context->treasure = json_to_treasure(json_treasure);
    }

    return context;
}
