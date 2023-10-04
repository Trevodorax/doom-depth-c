#include "player.h"
#include "../spell/spell.h"
#include <string.h>

player_t * create_player(char *name) {
    player_t * player = malloc(sizeof(player_t));
    player->name = malloc(sizeof(char)*strlen(name)+1);
    strcpy(player->name, name);

    player->hp = 200u;
    player->hp_max = 200u;
    player->mana = 100u;
    player->mana_max = 100u;
    player->xp = 0u;
    player->level = 1u;
    player->base_attack = 5u;
    player->base_defense = 2u;
    player->gold = 100u;
    player->action_points = (unsigned short)3;

    // TODO : add one spell at least for the beginning, see with @TomBrd
    player->offensive_spell = NULL;
    player->defensive_spell = NULL;
    player->healing_spell = NULL;

    player->chosen_armor = NULL;
    player->chosen_weapon = NULL;

    player->inventory = create_inventory();
    player->stats = create_stats();

    return player;
}

unsigned int compute_xp_needed(unsigned int level){
    int total_xp = 0;
    for (int i = 1; i <= level; ++i) {
        total_xp += 100 * i;
    }
    return (unsigned int)total_xp;
}

void level_up(player_t * player) {

}

void check_level_up(player_t * player){
    if(compute_xp_needed(player->level+1) <= player->xp){
        level_up(player);
    }
}

void give_exp(player_t * player, unsigned int amount){
    player->xp += amount;
    check_level_up(player);
}


