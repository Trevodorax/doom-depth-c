#ifndef PLAYER
#define PLAYER

#include "armor.h"
#include "inventory.h"
#include "spell.h"
#include "stats.h"
#include "weapon.h"

typedef struct {
    char *name;
    int hp;
    int mana;
    int manaMax;
    int xp;
    int level;
    int baseAttack;
    int baseDefense;
    int gold;
    Spell offensiveSpell;
    Spell defensiveSpell;
    Spell healingSpell;
    Armor *chosenArmor;
    Weapon *chosenWeapon;
    Inventory *inventory;
    Stats *stats;
} Player;

#endif