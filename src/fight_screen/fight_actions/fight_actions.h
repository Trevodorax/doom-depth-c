#ifndef DOOM_DEPTH_C_FIGHT_ACTIONS_H
#define DOOM_DEPTH_C_FIGHT_ACTIONS_H

#include "../fight_screen.h"

typedef enum {
    FA_END_TURN,
    FA_QUIT,
    FA_NOTHING
} fight_action_type_t;

int heal_potion(fight_context_t * fight_context, void * custom_params);
int mana_potion(fight_context_t * fight_context, void * custom_params);

int end_turn(fight_context_t * fight_context, void * custom_params);

int attack_spell(fight_context_t * fight_context, void * custom_params);
int defend_spell(fight_context_t * fight_context, void * custom_params);
int heal_spell(fight_context_t * fight_context, void * custom_params);

int attack_weapon(fight_context_t * fight_context, void * custom_params);

#endif //DOOM_DEPTH_C_FIGHT_ACTIONS_H
