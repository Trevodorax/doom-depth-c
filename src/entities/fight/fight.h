#ifndef FIGHT_H
#define FIGHT_H

#include "../../storage/json/json.h"

typedef struct {
    unsigned int nb_enemies;
    float *enemy_chances_to_appear;
} fight_t;

fight_t * json_to_fight(Json * fight_json);

#endif
