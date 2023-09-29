#ifndef STATS
#define STATS

typedef struct {
    unsigned int nb_monsters_killed;
    unsigned int nb_deaths;
    unsigned int damages_dealt;
    unsigned int health_healed;
    unsigned int max_level_reached;
} stats_t;

stats_t *create_stats();

stats_t *create_stats_from_database(unsigned int nb_monsters_killed, unsigned int nb_deaths, unsigned int damages_dealt,
                                    unsigned int health_healed, unsigned int max_level_reached);

#endif