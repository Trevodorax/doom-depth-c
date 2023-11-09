#ifndef DOOM_DEPTH_C_FIGHT_SCREEN_TYPES_H
#define DOOM_DEPTH_C_FIGHT_SCREEN_TYPES_H

#include "../entities/player/player.h"
#include "../logs/log.h"
#include "../entities/treasure/treasure.h"

typedef struct {
    player_t * player;
    list_t * monsters;
    char * notification_message;
    bool player_turn;
    treasure_t * treasure;
} fight_context_t;

typedef struct {
    int (*callback)(fight_context_t *, void * custom_params);
    void * params;
} fight_action_t;

#endif //DOOM_DEPTH_C_FIGHT_SCREEN_TYPES_H
