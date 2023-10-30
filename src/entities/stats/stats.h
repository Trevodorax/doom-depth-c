#ifndef STATS
#define STATS

#include "../../utils/array.h"
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int id;
    unsigned int nb_monsters_killed;
    unsigned int nb_deaths;
    unsigned int damages_dealt;
    unsigned int health_healed;
    unsigned int max_level_reached;
} stats_t;

/**
* @brief Creates a new stats struct with default values (on start of the game).
*
* This function allocates memory for a new stats struct and initializes its fields to default values.
*
* @return   Returns the new struct stats.
*
* @sideeffects Allocates memory for the stats.
* @dependencies None.
* @errors None.
*/
stats_t *create_stats();

/**
 * @brief Creates a stats struct in the database.
 *
 * This function creates a stats struct in the database.
 *
 * @param db
 * @param stats
 * @return
 *
 * @sideeffects May modify the SQLite database by inserting new data.
 * @dependencies Depends on the SQLite3 library.
 * @errors May return SQLITE_ERROR or other error codes if the query execution fails.
 */
int create_stats_in_db(sqlite3 *db, stats_t *stats);

/**
 * @brief Creates a stats struct from the database.
 *
 * This function allocates memory for a new stats struct and initializes its fields with the values from the database.
 *
 * @param stmt
 * @return
 *
 * @sideeffects Allocates memory for the stats.
 * @dependencies None.
 * @errors None.
 */
void *create_stats_from_db(sqlite3_stmt *stmt);

/**
 * @brief Saves the stats of a player in the database.
 *
 * This function updates the stats of a player in the database.
 *
 * @param db
 * @param stats
 * @param player_id
 * @return
 *
 * @sideeffects None.
 * @dependencies None.
 * @errors None.
 */
int save_stats(sqlite3 *db, stats_t *stats, int player_id);

/**
 * @brief Frees the stats
 * @param stats The stats to free
 */
void free_stats(stats_t *stats);

#endif