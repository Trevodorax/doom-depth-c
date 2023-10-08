#include <stdlib.h>
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

void *create_inventory_from_db(sqlite3_stmt *stmt) {

    inventory_t *inventory = malloc(sizeof(inventory_t));

    if (!inventory) {
        fprintf(stderr, "Failed to allocate memory for inventory.\n");
        return NULL;
    }

    inventory->capacity = sqlite3_column_int(stmt, 0);
    inventory->nb_weapons = sqlite3_column_int(stmt, 1);
    inventory->nb_armors = sqlite3_column_int(stmt, 2);
    inventory->nb_mana_potions = sqlite3_column_int(stmt, 3);
    inventory->nb_health_potions = sqlite3_column_int(stmt, 4);
    inventory->armorsHead = NULL;
    inventory->weaponsHead = NULL;

    return inventory;
}