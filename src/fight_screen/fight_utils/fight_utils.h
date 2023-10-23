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

/**
 * @brief Sets a notification message for a fight and prints it.
 *
 * @param fight_context The fight context in which the notification is displayed.
 * @param message The notification message to set and display.
 */
void build_notification(fight_context_t * fight_context, char * message);

/**
 * @brief Sets a formatted notification message for a fight and prints it.
 *
 * @param fight_context The fight context in which the notification is displayed.
 * @param message The formatted message string.
 * @param ... Variable arguments for message formatting.
 */
void build_notification_formatted(fight_context_t * fight_context, char * message, ...);

/**
 * @brief Finds the index in a probability array based on a random number.
 *
 * @param rand_num The random number generated.
 * @param probs The array of probabilities.
 * @param size The size of the probability array.
 * @return The index corresponding to the random number or -1 if no match is found.
 */
int find_index(int rand_num, const int probs[], int size);

/**
 * @brief Fetches a monster based on its name. (Temporary function)
 *
 * @param name The name of the monster.
 * @return A pointer to the monster struct or NULL if no match is found.
 */
void * get_monster_with_name(char * name);

/**
 * @brief Displays the current and maximum action points of a player.
 *
 * @param player The player whose action points are to be displayed.
 */
void debug_action_points(player_t *player);

/**
 * @brief Checks if a player has sufficient action points and deducts the required amount if possible.
 *
 * @param player The player whose action points are being checked.
 * @param amount The amount of action points required.
 * @return Returns true if the player has enough action points and they are deducted, false otherwise.
 */
bool check_and_remove_action_points(player_t *player, unsigned int amount);

/**
 * @brief creates the fight context out of the fight info
 *
 * @param fight The infos for the fight
 * @param player The player in the fight
 * @return The built fight context
 */
fight_context_t * build_fight_context(fight_t * fight, player_t * player);

#endif //DOOM_DEPTH_C_FIGHT_UTILS_H
