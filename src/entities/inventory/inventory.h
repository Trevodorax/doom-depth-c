#ifndef INVENTORY
#define INVENTORY

#include "../weapon/weapon.h"
#include "../armor/armor.h"
#include "../../utils/array.h"
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    unsigned int capacity;
    unsigned int nb_weapons;
    unsigned int nb_armors;
    unsigned int nb_mana_potions;
    unsigned int nb_health_potions;
    array_node_t *armorsHead;
    array_node_t *weaponsHead;
} inventory_t;

inventory_t *create_inventory();

/**
 * @brief Creates an inventory from the database.
 *
 * This function queries the database for an inventory with the given ID and constructs
 * an inventory with details fetched from the database.
 *
 * @param stmt  The SQLite statement which contains the inventory's data.
 * @return   Returns a pointer to the inventory with the given ID, or NULL if there was an error
 *           or no inventory was found.
 *
 * @sideeffects Allocates memory for the inventory.
 * @dependencies Depends on the SQLite library.
 * @errors Errors during database operations or memory allocation can result in NULL being returned.
 */
void *create_inventory_from_db(sqlite3_stmt *stmt);

#endif