#ifndef MONSTER
#define MONSTER

#include "../../utils/array.h"
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
 * @brief Creates a linked list of monsters from the database.
 *
 * This function queries the database for all monsters and constructs a linked list
 * where each node represents a monster with details fetched from the database.
 *
 * @param db  The SQLite database connection which contains the monsters' data.
 * @return   Returns a pointer to the head of the linked list containing monsters,
 *           or NULL if there was an error or no monsters were found.
 *
 * @sideeffects Allocates memory for the linked list and its nodes.
 * @dependencies Depends on the SQLite library and the provided linked list functions.
 * @errors Errors during database operations or memory allocation can result in NULL being returned.
 */
array_node_t *create_monsters_from_db(sqlite3 *db);

#endif