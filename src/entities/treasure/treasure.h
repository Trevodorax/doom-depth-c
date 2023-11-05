#include "../../storage/json/json.h"
#include "../player/player.h"

#ifndef DOOM_DEPTH_C_TREASURE_H
#define DOOM_DEPTH_C_TREASURE_H

typedef struct {
    int coins;
    // TODO: add other things in there (weapons, armors, etc.) @noam
} treasure_t;

/**
 * @brief Converts a json into a treasure
 *
 * @param treasure_json The json to convert
 * @return The treasure, or NULL if the json had a bad shape
 */
treasure_t * json_to_treasure(json_t * treasure_json);

/**
 * @brief Puts the details of a treasure on a json object
 *
 * @param object The object to add the treasure to
 * @param treasure The treasure to get the info of
 */
void add_treasure_to_json_object(json_t * object, treasure_t * treasure);

/**
 * @brief Gives a treasure to a player
 *
 * @param treasure The treasure to give
 * @param player The player to give the treasure to
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int give_treasure_to_player(treasure_t * treasure, player_t * player);

#endif //DOOM_DEPTH_C_TREASURE_H
