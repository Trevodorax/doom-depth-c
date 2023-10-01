#ifndef PLAYER
#define PLAYER

#include "../armor/armor.h"
#include "../inventory/inventory.h"
#include "../spell/spell.h"
#include "../stats/stats.h"
#include "../weapon/weapon.h"

typedef struct {
    char *name;
    unsigned int hp;
    unsigned int mana;
    unsigned int mana_max;
    unsigned int xp;
    unsigned int level;
    unsigned int base_attack;
    unsigned int base_defense;
    unsigned int gold;
    Spell offensive_spell;
    Spell defensive_spell;
    Spell healing_spell;
    Armor *chosen_armor;
    Weapon *chosen_weapon;
    Inventory *inventory;
    Stats *stats;
} player_t;

#endif
