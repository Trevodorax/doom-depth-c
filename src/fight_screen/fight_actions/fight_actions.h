#ifndef DOOM_DEPTH_C_FIGHT_ACTIONS_H
#define DOOM_DEPTH_C_FIGHT_ACTIONS_H

#include "../fight_screen.h"

int quit(fight_context_t * fight_context, void * custom_params);

int potion(fight_context_t * fight_context, void * custom_params);

typedef struct {
    monster_t * target;

    int test_number;
} attack_spell_params_t;
int attack_spell(fight_context_t * fight_context, void * custom_params);

typedef struct {
    monster_t * target;
} attack_weapon_params;
int attack_weapon(fight_context_t * fight_context, void * custom_params);

#endif //DOOM_DEPTH_C_FIGHT_ACTIONS_H
