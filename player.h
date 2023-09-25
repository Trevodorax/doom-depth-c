#ifndef PLAYER
#define PLAYER

#include "armor.h"
#include "inventory.h"
#include "spell.h"
#include "stats.h"
#include "weapon.h"

typedef struct {
    char *name;
    unsigned int hp;
    unsigned int mana;
    unsigned int manaMax;
    unsigned int xp;
    unsigned int level;
    unsigned int baseAttack;
    unsigned int baseDefense;
    unsigned int gold;
    Spell offensiveSpell;
    Spell defensiveSpell;
    Spell healingSpell;
    Armor *chosenArmor;
    Weapon *chosenWeapon;
    Inventory *inventory;
    Stats *stats;
} Player;

#endif