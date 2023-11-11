#ifndef PLAYER
#define PLAYER

#include "../armor/armor.h"
#include "../inventory/inventory.h"
#include "../spell/spell.h"
#include "../stats/stats.h"
#include "../weapon/weapon.h"
#include "../../utils/array.h"
#include "../../storage/database/database.h"
#include "../../storage/database/database_queries.h"
#include "../entities.h"
#include <sqlite3.h>
#include "../monster/monster.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int id;
    char * name;
    unsigned int hp;
    unsigned int hp_max;
    unsigned int mana;
    unsigned int mana_max;
    unsigned int xp;
    unsigned int level;
    unsigned int base_attack;
    unsigned int base_defense;
    unsigned int gold;
    unsigned short action_points;
    unsigned short max_action_points;
    bool is_defending;
    spell_t * offensive_spell;
    spell_t * defensive_spell;
    spell_t * healing_spell;
    armor_t * chosen_armor;
    weapon_t * chosen_weapon;
    inventory_t * inventory;
    stats_t * stats;
    char * current_map;
} player_t;

player_t * create_player(char * name);

/**
 * @brief Creates a player in the database.
 *
 * This function creates a player in the database.
 *
 * @param player A pointer to the player to be created.
 *
 * @return Last inserted row id.
 * @sideeffects May modify the SQLite database by inserting new data.
 * @dependencies Depends on the SQLite3 library.
 * @errors May return SQLITE_ERROR or other error codes if the query execution fails.
 */
int create_player_in_db(player_t * player);

/**
 * @brief Creates a player from the database.
 *
 * This function creates a player from the database.
 *
 * @param stmt A pointer to the SQLite3 statement.
 * @param spells A pointer to the array of spells.
 *
 * @return A pointer to the player created from the database.
 * @sideeffects May modify the SQLite database by inserting new data.
 * @dependencies Depends on the SQLite3 library.
 * @errors May return SQLITE_ERROR or other error codes if the query execution fails.
 */
void *create_player_from_db(sqlite3_stmt *stmt);

/**
 * @brief Saves the player's data to the database.
 *
 * This function saves the player's data to the database.
 *
 * @param db A pointer to the SQLite3 database connection.
 * @param player A pointer to the player whose data is to be saved.
 *
 * @return SQLITE_OK if the player's data is successfully saved. Otherwise, it returns an error code.
 * @sideeffects May modify the SQLite database by inserting new data.
 * @dependencies Depends on the SQLite3 library.
 * @errors May return SQLITE_ERROR or other error codes if the query execution fails.
 */
int save_player(sqlite3 *db, player_t *player);

/**
 * @brief Heals the player by the given amount.
 *
 * @param player
 * @param amount
 * @return The number of hp gained by the player
 */
unsigned int heal_player(player_t * player, unsigned int amount);

/**
 * @brief Checks if a player has to level up
 *
 * @param player The player to check
 * @return If the player levels up
 */
bool check_level_up(player_t * player);

/**
 * @brief gives experience point to the player and levels up if necessary
 *
 * @param player
 * @param amount
 */
void give_exp(player_t * player, unsigned int amount);

/**
 * @brief Gives mana to the player in the given amount.
 *
 * @param player
 * @param amount
 * @return The number of mana points gained by the player
 */
void heal_mana(player_t * player, unsigned int amount);

/**
 * @brief Frees the player
 * @param player The player to free
 */
void free_player(player_t *player);

/**
 * @brief Creates an array of players from the database.
 *
 * This function creates an array with the three saves the player can have.
 *
 * @param db A pointer to the SQLite3 database connection.
 *
 * @return A pointer to the array of players created from the database.
 * @sideeffects May modify the SQLite database by inserting new data.
 * @dependencies Depends on the SQLite3 library.
 * @errors May return SQLITE_ERROR or other error codes if the query execution fails.
 */
array_node_t *get_players_from_db(sqlite3 *db);

/**
 * @brief Saves or retrieves a checkpoint for the state of the user
 *
 * @param player The player
 * @param save true if saving, false if retrieving
 */
void player_state_checkpoint(player_t * player, bool save);

/**
 * @brief Deletes a player from the database.
 *
 * This function deletes a player from the database.
 *
 * @param db A pointer to the SQLite3 database connection.
 * @param player A pointer to the player to be deleted.
 *
 * @return SQLITE_OK if the player is successfully deleted. Otherwise, it returns an error code.
 * @sideeffects May modify the SQLite database by inserting new data.
 * @dependencies Depends on the SQLite3 library.
 * @errors May return SQLITE_ERROR or other error codes if the query execution fails.
 */
int delete_player(sqlite3 * db, player_t * player);

#endif
