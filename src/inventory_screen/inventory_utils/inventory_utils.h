#ifndef DOOM_DEPTH_C_INVENTORY_UTILS_H
#define DOOM_DEPTH_C_INVENTORY_UTILS_H

#include "../../entities/armor/armor.h"
#include "../../entities/weapon/weapon.h"

/**
 * @brief creates a string containing the details of an armor, in multiple lines :
 * name,
 * rarity,
 * defense value,
 * uses left before it breaks,
 * cost
 *
 * @param armor the armor from which to get the details
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
char *armor_details_to_string(armor_t *armor);

/**
 * @brief creates a string containing the details of a weapon, in multiple lines :
 * name,
 * rarity,
 * number of attacks per turn it gives,
 * attack range,
 * uses left before it breaks,
 * cost
 *
 * @param weapon the weapon from which to get the details
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
char *weapon_details_to_string(weapon_t *weapon);

/**
 * @brief creates a string containing the details on the health potions in an inventory :
 * name (health potion),
 * healing value (20),
 * the number of health potions available
 *
 * @param quantity the number of health potions in the inventory
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
char *health_potions_details_to_string(unsigned int quantity);

/**
 * @brief creates a string containing the details on the mana potions in an inventory :
 * name (mana potion),
 * mana value (20),
 * the number of mana potions available
 *
 * @param quantity the number of mana potions in the inventory
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
char *mana_potions_details_to_string(unsigned int quantity);

#endif