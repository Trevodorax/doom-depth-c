#ifndef DOOM_DEPTH_C_STAGE_H
#define DOOM_DEPTH_C_STAGE_H
#include <stdbool.h>
#include "../../entities/fight/fight.h"
#include "../../storage/json/json.h"

typedef enum {
    EMPTY,
    FIGHT,
    SHOP,
    TREASURE
} stage_type_t;

typedef struct stage_t stage_t;
struct stage_t {
    stage_type_t type;

    bool is_start; // true if the stage is where the player is placed when entering the map
    bool is_done; // true if the stage is not a fight or if the fight is done

    bool has_linked_map; // true if the user goes to a linked map when stepping on this stage
    char * linked_map_file_path;

    fight_t *fight; // only if type == fight

    // next stages
    stage_t *top;
    stage_t *right;
    stage_t *bottom;
    stage_t *left;
};

stage_t * json_to_stage(Json * json_stage);

#endif //DOOM_DEPTH_C_STAGE_H
