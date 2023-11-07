#ifndef DOOM_DEPTH_C_FIGHT_SCREEN_H
#define DOOM_DEPTH_C_FIGHT_SCREEN_H

#include "../entities/player/player.h"
#include "../entities/monster/monster.h"
#include "../entities/fight/fight.h"
#include "../ui_utils/sdl_utils/sdl_utils.h"
#include "../game_window/game_window.h"
#include "./types.h"

/**
 * @brief Screen for fights
 *
 * @param game_window
 * @param player The player doing the fight
 * @param stage The fight data used to randomly create a fight
 * @return 0 if game over, 1 if victory
 */
router_t fight_screen(game_window_t * game_window, player_t * player, stage_t *stage);

void build_notification(fight_context_t * fight_context, char * message);
void build_notification_formatted(fight_context_t * fight_context, char * message, ...);

#endif //DOOM_DEPTH_C_FIGHT_SCREEN_H
