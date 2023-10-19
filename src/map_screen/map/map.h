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
map_t * json_to_map(Json * json_map);

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

#endif //DOOM_DEPTH_C_MAP_H
