#include <malloc.h>
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