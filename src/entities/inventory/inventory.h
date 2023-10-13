#ifndef INVENTORY
#define INVENTORY

#include "../weapon/weapon.h"
#include "../armor/armor.h"
#include "../../utils/array.h"

typedef struct {
    unsigned int capacity;
    unsigned int nb_mana_potions;
    unsigned int nb_health_potions;
    unsigned int nb_weapons;
    unsigned int nb_armors;
    array_node_t *armors_head;
    array_node_t *weapons_head;
} inventory_t;

inventory_t *create_inventory();

inventory_t *create_inventory_from_database(
        unsigned int capacity,
        unsigned int nb_mana_potions,
        unsigned int nb_health_potions,
        unsigned int nb_weapons,
        unsigned int nb_armors,
        array_node_t *armors_head,
        array_node_t *weapons_head
);

#endif