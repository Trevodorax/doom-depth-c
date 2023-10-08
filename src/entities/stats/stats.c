#include "stats.h"

stats_t * create_stats(){
    stats_t * stats = malloc(sizeof(stats_t));
    stats->damages_dealt = 0u;
    stats->health_healed = 0u;
    stats->max_level_reached = 0u;
    stats->nb_deaths = 0u;
    stats->nb_monsters_killed = 0u;
    return stats;
}

void *create_stats_from_db(sqlite3_stmt *stmt) {

    stats_t *stats = malloc(sizeof(stats_t));

    if (!stats) {
        fprintf(stderr, "Failed to allocate memory for stats.\n");
        return NULL;
    }

    stats->id = sqlite3_column_int(stmt, 0);
    stats->nb_monsters_killed = sqlite3_column_int(stmt, 1);
    stats->nb_deaths = sqlite3_column_int(stmt, 2);
    stats->damages_dealt = sqlite3_column_int(stmt, 3);
    stats->health_healed = sqlite3_column_int(stmt, 4);
    stats->max_level_reached = sqlite3_column_int(stmt, 5);

    return stats;

}