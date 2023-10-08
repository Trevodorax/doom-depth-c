#ifndef SPELL
#define SPELL

#include "../../utils/array.h"
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

typedef enum SpellType {
    HEALING = 1, DEFENSE = 2, ATTACK = 3
} spell_type_t;

typedef struct {
    int id;
    spell_type_t type;
    char *name;
    unsigned int amount;
    unsigned int cost;
} spell_t;

/**
 * @brief Creates a spell from the database.
 *
 * This function queries the database for a spell with the given ID and constructs
 * a spell with details fetched from the database.
 *
 * @param stmt  The SQLite statement which contains the spell's data.
 * @return   Returns a pointer to the spell with the given ID, or NULL if there was an error
 *           or no spell was found.
 *
 * @sideeffects Allocates memory for the spell.
 * @dependencies Depends on the SQLite library.
 * @errors Errors during database operations or memory allocation can result in NULL being returned.
 */
void *create_spell_from_db(sqlite3_stmt *stmt);


#endif