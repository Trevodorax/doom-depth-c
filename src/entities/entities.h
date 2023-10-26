#ifndef DOOM_DEPTH_C_ENTITIES_H
#define DOOM_DEPTH_C_ENTITIES_H

#include <sqlite3.h>
#include "spell/spell.h"
#include "weapon/weapon.h"
#include "armor/armor.h"
#include "monster/monster.h"
#include "../storage/database/database.h"
#include "../utils/array.h"

/**
 * @brief Initializes the entities.
 *
 * This function initializes the entities by calling the init functions of the
 * individual entities.
 *
 * @param db  The database to initialize the entities from.
 *
 * @sideeffects Allocates memory for the entities.
 * @dependencies Depends on the SQLite library.
 * @errors Errors during database operations or memory allocation can result in NULL being returned.
 */
void init_entities(sqlite3 *db);

/**
 * @brief Gets the weapons.
 *
 * This function returns the weapons.
 *
 * @return   Returns the weapons.
 *
 * @sideeffects None.
 * @dependencies None.
 * @errors None.
 */
array_node_t *get_weapons();

/**
 * @brief Gets the spells.
 *
 * This function returns the spells.
 *
 * @return   Returns the spells.
 *
 * @sideeffects None.
 * @dependencies None.
 * @errors None.
 */
array_node_t *get_spells();

/**
 * @brief Gets the armors.
 *
 * This function returns the armors.
 *
 * @return   Returns the armors.
 *
 * @sideeffects None.
 * @dependencies None.
 * @errors None.
 */
array_node_t *get_armors();

/**
 * @brief Gets the monsters.
 *
 * This function returns the monsters.
 *
 * @return   Returns the monsters.
 *
 * @sideeffects None.
 * @dependencies None.
 * @errors None.
 */
array_node_t *get_monsters();

monster_t* get_monster_by_name(char* name);


#endif //DOOM_DEPTH_C_ENTITIES_H
