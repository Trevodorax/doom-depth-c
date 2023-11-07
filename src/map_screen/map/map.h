#ifndef DOOM_DEPTH_C_MAP_H
#define DOOM_DEPTH_C_MAP_H

#include "../stage/stage.h"

typedef struct {
    char * name;
    stage_t * first_stage;
} map_t;

/**
 * @brief Converts a json into a map (does all the required checks)
 *
 * @param json_map The json to convert into a map
 * @error If the json is not a correct map, will return NULL
 * @return The parsed map or NULL
 */
map_t * json_to_map(json_t * json_map);

/**
 * @brief Gets the height and width of a map
 *
 * @param map The map for which we get the dimensions
 * @param width Where the width will be stored
 * @param height Where the height will be stored
 * @param initial_x Where the x-coord of the first stage will be stored
 * @param initial_y Where the y-coord of the first stage will be stored
 * @sideeffects Sets the width and height to the dimensions of the map
 * @error If the map is not correct, this function will die (the checks must have been done before)
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int get_map_dimensions(map_t *map, int * width, int * height, int * initial_x, int * initial_y);

/**
 * @brief Retrieves the map from the given file
 *
 * @param file_path The path of the map file
 * @return The map, or NULL in case of error
 */
map_t * get_map_from_file(char * file_path);

/**
 * @brief Frees the map
 * @param map The map to free
 */
void free_map(map_t * map);

/**
 * @brief Creates a json version of a map
 *
 * @param map The map to convert
 * @return The json version
 */
json_t * map_to_json(map_t * map);

/**
 * @brief Get the map from a player
 * @param player The player to get the map for
 *
 * @return The map
 */
map_t * get_player_map(player_t * player);

/**
 * @brief Saves the map of the player
 *
 * @param player The player to save the map for
 * @param map The saved map
 */
void save_player_map(player_t * player, map_t * map);

#endif //DOOM_DEPTH_C_MAP_H
