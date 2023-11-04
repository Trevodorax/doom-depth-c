#ifndef WEAPON_H
#define WEAPON_H

#include "../../utils/array.h"
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int id;
    char * name;
    unsigned int min_attack;
    unsigned int max_attack;
    unsigned int attacks_per_turn;
    short rarity;
    unsigned int max_uses;
    unsigned int uses;
    unsigned int cost;
    char * image_path;
    char * ascii_path;
    int chosen;
    int id_in_inventory;
} weapon_t;

/**
 * @brief Creates a weapon from the database.
 *
 * This function queries the database for a weapon with the given ID and constructs
 * a weapon with details fetched from the database.
 *
 * @param stmt  The SQLite statement which contains the weapon's data.
 * @return   Returns a pointer to the weapon with the given ID, or NULL if there was an error
 *           or no weapon was found.
 *
 * @sideeffects Allocates memory for the weapon.
 * @dependencies Depends on the SQLite library.
 * @errors Errors during database operations or memory allocation can result in NULL being returned.
 */
void *create_weapon_from_db(sqlite3_stmt *stmt);

/**
 * @brief Frees the weapon
 * @param weapon The weapon to free
 */
void free_weapon(weapon_t *weapon);

#endif