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

void *create_stats_from_db(sqlite3_stmt *stmt);

#endif