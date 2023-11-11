#ifndef DOOM_DEPTH_C_GAME_STATS_SCREEN_TYPES_H
#define DOOM_DEPTH_C_GAME_STATS_SCREEN_TYPES_H

typedef enum {
    GAME_STAT_DAMAGES_DEALT,
    GAME_STAT_HEALTH_HEALED,
    GAME_STAT_NB_DEATHS,
    GAME_STAT_NB_MONSTERS_KILLED,
    GAME_STAT_COUNT
} game_stat_t;

static char * game_stat_display_name[GAME_STAT_COUNT] = {
        "Damages dealt",
        "Health healed",
        "Number of deaths",
        "Number of monsters killed"
};

#endif