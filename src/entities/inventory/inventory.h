#ifndef INVENTORY
#define INVENTORY

#include "../../storage/database/database.h"
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
    array_node_t *armors_head;
    array_node_t *weapons_head;
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

/**
 * @brief Creates a full inventory from the database.
 *
 * This function queries the database for an inventory with the given ID and constructs
 * an inventory with details fetched from the database.
 *
 * @param db    The SQLite database connection.
 * @param player_id The ID of the player whose inventory is to be fetched.
 * @return   Returns a pointer to the inventory with the given ID, or NULL if there was an error
 *           or no inventory was found.
 *
 * @sideeffects Allocates memory for the inventory.
 * @dependencies Depends on the SQLite library.
 * @errors Errors during database operations or memory allocation can result in NULL being returned.
 */
array_node_t *create_full_inventory_from_db(sqlite3 *db, int player_id);

/**
 * @brief Saves the inventory's data to the database.
 *
 * This function saves the inventory's data to the database.
 *
 * @param db A pointer to the SQLite3 database connection.
 * @param inventory A pointer to the inventory whose data is to be saved.
 * @param player_id The ID of the player whose inventory is to be saved.
 * @return Returns SQLITE_OK if the query was executed successfully, or an error code otherwise.
 *
 * @sideeffects May modify the SQLite database by inserting new data.
 * @dependencies Depends on the SQLite3 library.
 * @errors May return SQLITE_ERROR or other error codes if the query execution fails.
 */
int save_inventory(sqlite3 *db, inventory_t *inventory, int player_id);

/**
 * @brief Gets the chosen weapon from the inventory.
 *
 * This function gets the chosen weapon from the inventory.
 *
 * @param inventory A pointer to the inventory.
 * @return Returns a pointer to the chosen weapon.
 *
 * @sideeffects None.
 * @dependencies None.
 * @errors None.
 */
weapon_t *get_chosen_weapon(inventory_t *inventory);

/**
 * @brief Gets the chosen armor from the inventory.
 *
 * This function gets the chosen armor from the inventory.
 *
 * @param inventory A pointer to the inventory.
 * @return Returns a pointer to the chosen armor.
 *
 * @sideeffects None.
 * @dependencies None.
 * @errors None.
 */
armor_t *get_chosen_armor(inventory_t *inventory);


#endif