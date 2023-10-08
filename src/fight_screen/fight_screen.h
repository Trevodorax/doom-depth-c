#ifndef DOOM_DEPTH_C_FIGHT_SCREEN_H
#define DOOM_DEPTH_C_FIGHT_SCREEN_H

#include "../entities/player/player.h"
#include "../entities/monster/monster.h"
#include "../entities/fight/fight.h"
#include "../sdl_utils/sdl_utils.h"
#include "../game_window/game_window.h"


typedef struct {

    player_t * player;


    array_node_t * monsters;

    // Optional : only when
    monster_t * attack_target;
} fight_context_t;

typedef struct {
    int (*callback)(fight_context_t *, void * custom_params);
    void * params;
} fight_action_t ;

/**
 * @brief Screen for fights
 *
 * @param game_window
 * @param player The player doing the fight
 * @param fight The fight data used to randomly create a fight
 * @return 0 if game over, 1 if victory
 */
int fight_screen(game_window_t * game_window, player_t * player, fight_t * fight);

#endif //DOOM_DEPTH_C_FIGHT_SCREEN_H
