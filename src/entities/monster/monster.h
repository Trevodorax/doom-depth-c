#ifndef MONSTER
#define MONSTER

#include "../../utils/array.h"
#include "../../storage/database/database.h"
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
    char *image;
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

#endif