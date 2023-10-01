#include <malloc.h>
#include "inventory.h"

inventory_t * create_inventory(){
    inventory_t * inv = malloc(sizeof(inventory_t));
    inv->armorsHead = NULL;
    inv->weaponsHead = NULL;
    inv->nb_armors = 0u;
    inv->nb_weapons = 0u;
    inv->nb_health_potions = 1u;
    inv->nb_mana_potions = 0u;
    inv->capacity = 10u;
    return inv;
}