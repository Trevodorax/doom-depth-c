#ifndef PLAYER
#define PLAYER

#include "../armor/armor.h"
#include "../inventory/inventory.h"
#include "../spell/spell.h"
#include "../stats/stats.h"
#include "../weapon/weapon.h"
#include <stdlib.h>
#include <string.h>

typedef struct player_t {
    int id;
    char *name;
    unsigned int hp;
    unsigned int hp_max;
    unsigned int mana;
    unsigned int mana_max;
    unsigned int xp;
    unsigned int level;
    unsigned int base_attack;
    unsigned int base_defense;
    unsigned int gold;
    unsigned short action_points;
    spell_t *offensive_spell;
    spell_t *defensive_spell;
    spell_t *healing_spell;
    armor_t *chosen_armor;
    weapon_t *chosen_weapon;
    inventory_t *inventory;
    stats_t *stats;
} player_t;

player_t *create_player(char *name);

player_t *
create_player_from_database(char *name, unsigned int hp, unsigned int mana, unsigned int mana_max, unsigned int xp,
                            unsigned int level, unsigned int base_attack, unsigned int base_defense, unsigned int gold,
                            unsigned int action_points, spell_t *offensive_spell, spell_t *defensive_spell,
                            spell_t *healing_spell, armor_t *chosen_armor, weapon_t *chosen_weapon, inventory_t *inventory,
                            stats_t *stats);

void give_exp(player_t * player, unsigned int amount);


#endif
