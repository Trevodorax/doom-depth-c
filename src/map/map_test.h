#ifndef DOOM_DEPTH_C_MAP_TEST_H
#define DOOM_DEPTH_C_MAP_TEST_H

#include "../storage/json/json.h"
#include "map.h"

int map_test();
map_t * json_to_map(Json * json_map);

#endif //DOOM_DEPTH_C_MAP_TEST_H
