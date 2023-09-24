#ifndef INVENTORY
#define INVENTORY

#include "weapon.h"

typedef struct {
    int capacity;
    int nbManaPotions;
    int nbHealthPotions;
    int nbWeapons;
    int nbArmors;
    Weapon *weapons;
} Inventory;

#endif