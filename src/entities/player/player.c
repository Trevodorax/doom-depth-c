#include "player.h"
#include "../spell/spell.h"

player_t * create_player(char *name) {
    player_t * player = malloc(sizeof(player_t));
    player->name = malloc(sizeof(char)*strlen(name)+1);
    strcpy_s(player->name,strlen(name),name);

    player->hp = 200u;
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

