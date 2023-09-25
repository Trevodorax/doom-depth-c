#ifndef INVENTORY
#define INVENTORY

#include "../weapon/weapon.h"

typedef struct {
    unsigned int capacity;
    unsigned int nb_mana_potions;
    unsigned int nb_health_potions;
    unsigned int nb_weapons;
    unsigned int nb_armors;
    Weapon *weapons;
} Inventory;

#endif