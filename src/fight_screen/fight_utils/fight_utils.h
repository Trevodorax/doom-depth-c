#ifndef DOOM_DEPTH_C_FIGHT_UTILS_H
#define DOOM_DEPTH_C_FIGHT_UTILS_H

#include "../../entities/player/player.h"
#include "../types.h"
#include "../../entities/fight/fight.h"

/**
 * @brief Makes the player attack the given monster
 *
 * @param player
 * @param target
 * @return The damages inflicted to the monster
 */
unsigned int player_attack(player_t * player, monster_t * target);

fight_context_t * build_fight_context(fight_t * fight, player_t * player);

void build_notification(fight_context_t * fight_context, char * message);

void build_notification_formatted(fight_context_t * fight_context, char * message, ...);

int find_index(int rand_num, const int probs[], int size);

void * get_monster_with_name(char * name);

void debug_action_points(player_t *player);

bool check_and_remove_action_points(player_t *player, unsigned int amount);

#endif //DOOM_DEPTH_C_FIGHT_UTILS_H
