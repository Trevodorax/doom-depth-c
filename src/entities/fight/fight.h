#ifndef FIGHT_H
#define FIGHT_H

#include "../../storage/json/json.h"

typedef struct {
    unsigned int nb_enemies;
    float *enemy_chances_to_appear;
} fight_t;

/**
 * @brief Converts a json into a fight (does all the required checks)
 *
 * @param json_fight The json to convert into a fight
 * @error If the json is not a correct fight, will return NULL
 * @return The parsed fight or NULL
 */
fight_t * json_to_fight(Json * fight_json);

#endif
