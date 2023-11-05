#ifndef DOOM_DEPTH_C_SHOP_UTILS_H
#define DOOM_DEPTH_C_SHOP_UTILS_H

#define ITEMS_PER_PAGE 9
#define HEALTH_POTIONS_COST 10
#define MANA_POTIONS_COST 20

#include "../shop_screen.h"
#include "stdbool.h"
#include "../../utils/items_management/types.h"

/**
 * @brief Returns the number of items existing in the game in a given category
 *
 * @param category
 * @returns number of items in category or -1
 */
int get_items_count(category_options_t category);

/**
 * @brief Converts the quantity of gold a player has into a string
 *
 * @param player
 * @returns the quantity of gold a player has in a string
 */
char * player_gold_to_string(player_t * player);

/**
 * @brief Converts the details of an armor (useful for the shop) into a string
 *
 * @param armor
 * @returns the details of given armor in a string
 */
char * shop_armor_details_to_string(armor_t * armor);

/**
 * @brief Converts the details of a weapon (useful for the shop) into a string
 *
 * @param weapon
 * @returns the details of given armor in a string
 */
char * shop_weapon_details_to_string(weapon_t * weapon);

/**
 * @brief Converts the details of a health potion (useful for the shop) into a string
 *
 * @returns the details in a string
 */
char *shop_health_potions_details_to_string();

/**
 * @brief Converts the details of a health potion (useful for the shop) into a string
 *
 * @returns the details in a string
 */
char *shop_mana_potions_details_to_string();

#endif //DOOM_DEPTH_C_SHOP_UTILS_H