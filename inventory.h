#ifndef INVENTORY
#define INVENTORY

#include "weapon.h"

typedef struct {
    unsigned int capacity;
    unsigned int nbManaPotions;
    unsigned int nbHealthPotions;
    unsigned int nbWeapons;
    unsigned int nbArmors;
    Weapon *weapons;
} Inventory;

#endif