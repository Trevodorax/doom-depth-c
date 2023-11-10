#include "../map_screen/map/map.h"

#ifndef DOOM_DEPTH_C_MAP_GENERATION_H
#define DOOM_DEPTH_C_MAP_GENERATION_H

/**
 * @brief Generates a complete map
 *
 * @param prev_map The previous map that links to the new one
 * @return The generated map
 */
map_t * generate_map(map_t * prev_map);

/**
 * @brief Generates all the stages for a map
 *
 * @param previous_fight The last fight before these stages (to allow progressive map generation)
 * @return First stage of the created stages
 */
stage_t * generate_stages(fight_t * previous_fight);

/**
 * @brief Generates a stage and its neighbours recursively
 *
 * @param previous_fight The fight before this stage
 * @param anti_collision_map A map to make sure we don't put 2 stages at the same coords
 * @param x_coord X coord of the current stage
 * @param y_coord Y coord of the current stage
 * @return First stage of the created stages
 */
stage_t * generate_stages_rec(
        fight_t * previous_fight,
        bool ** anti_collision_map,
        int x_coord,
        int y_coord
    );

/**
 * @brief Gets an available map name
 *
 * Checks the given folder for map names by numbers.
 * When it gets one that is not already created, returns it
 *
 * @param maps_folder_path The folder where maps are stored
 * @return The available name
 */
char * get_available_map_name(char * maps_folder_path);

/**
 * @brief Generates a fight that is harder than the previous one
 *
 * @param previous_fight The previous fight
 * @return The generated fight
 */
fight_t * generate_harder_fight(fight_t * previous_fight);

/**
 * @brief Searches for the last done fight recursively
 *
 * @param last_stage The stage to start the search from
 * @return The last fight, or NULL if none was found
 */
fight_t * get_last_done_fight(stage_t * last_stage);

/**
 * @brief Generates a treasure
 *
 * The bigger the last fight found, the bigger the treasure will be
 *
 * @param treasure_stage The stage on which the treasure should be
 * @return The generated treasure
 */
treasure_t * generate_treasure(stage_t * treasure_stage);

#endif //DOOM_DEPTH_C_MAP_GENERATION_H
