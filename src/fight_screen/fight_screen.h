#ifndef DOOM_DEPTH_C_FIGHT_SCREEN_H
#define DOOM_DEPTH_C_FIGHT_SCREEN_H

#include "../entities/player/player.h"
#include "../entities/monster/monster.h"
#include "../entities/fight/fight.h"

typedef struct {
    player_t * player;
    monster_t * monsters;
} fight_context_t;

typedef int (*fight_action)(fight_context_t *);

/**
 * @brief Screen for fights
 *
 * @param player The player doing the fight
 * @param fight The fight data used to randomly create a fight
 * @return 0 if game over, 1 if victory
 */
int fight_screen(player_t * player, fight_t * fight);

#endif //DOOM_DEPTH_C_FIGHT_SCREEN_H
