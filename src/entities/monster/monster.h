#ifndef MONSTER
#define MONSTER

#include "../../utils/array.h"
#include "../../storage/database/database.h"
#include "../../storage/json/json.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    short type;
    char *name;
    unsigned int hp;
    unsigned int hp_max;
    unsigned int attack;
    unsigned int defense;
    char * image_path;
    char * ascii_path;
} monster_t;

/**
 * @brief Creates a monster from the database.
 *
 * This function queries the database for a monster with the given ID and constructs
 * a monster with details fetched from the database.
 *
 * @param stmt  The SQLite statement which contains the monster's data.
 * @return   Returns a pointer to the monster with the given ID, or NULL if there was an error
 *           or no monster was found.
 *
 * @sideeffects Allocates memory for the monster.
 * @dependencies Depends on the SQLite library.
 * @errors Errors during database operations or memory allocation can result in NULL being returned.
 */
void *create_monster_from_db(sqlite3_stmt *stmt);

/**
 * @brief Safely casts a void * into a monster_t *
 *
 * @param void_monster The pointer to cast
 * @return The casted pointer
 */
monster_t * void_to_monster(void * void_monster);

/**
 * @brief Copies a monster deeply
 *
 * @param monster The copied monster
 * @return A deep copy of the monster
 */
monster_t * copy_monster(const monster_t * monster);

/**
 * @brief Creates a json version of a monster
 *
 * @param monster The map to convert
 * @return The json version
 */
json_t * monster_to_json(monster_t *monster);

/**
 * @brief Creates a monster out of a json
 *
 * @param object The json object
 * @return The monster
 */
monster_t * json_to_monster(json_t * object);

#endif