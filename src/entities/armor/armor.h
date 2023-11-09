#ifndef ARMOR_H
#define ARMOR_H

#include "../../utils/array.h"
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int id;
    char * name;
    short rarity;
    unsigned int amount;
    unsigned int uses;
    unsigned int max_uses;
    unsigned int cost;
    char * image_path;
    char * ascii_path;
    int chosen;
    int id_in_inventory;
} armor_t;


/**
 * @brief Creates an armor from the database.
 *
 * This function queries the database for an armor with the given ID and constructs
 * an armor with details fetched from the database.
 *
 * @param stmt  The SQLite statement which contains the armor's data.
 * @return   Returns a pointer to the armor with the given ID, or NULL if there was an error
 *           or no armor was found.
 *
 * @sideeffects Allocates memory for the armor.
 * @dependencies Depends on the SQLite library.
 * @errors Errors during database operations or memory allocation can result in NULL being returned.
 */
void *create_armor_from_db(sqlite3_stmt *stmt);

/**
 * @brief Frees the armor
 * @param armor The armor to free
 */
void free_armor(armor_t *armor);

/**
 * @brief Safely casts a void * into a armor_t *
 *
 * @param void_armor The pointer to cast
 * @return The casted pointer
 */
armor_t *void_to_armor(void *void_armor);

/**
 * @brief Copies an armor deeply
 *
 * @param armor The copied armor
 * @return A deep copy of the armor
 */
armor_t * copy_armor(const armor_t * armor);
#endif
