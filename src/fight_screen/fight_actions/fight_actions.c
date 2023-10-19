#include "fight_actions.h"
#include <stdio.h>

int quit(fight_context_t * fight_context, void * custom_params) {
    printf("\nExit");
    return -1;
}

int potion(fight_context_t * fight_context, void * custom_params) {
    printf("\nDrink potion");
    return 0;
}

int attack_spell(fight_context_t * fight_context, void * custom_params) {
    attack_spell_params_t * params = (attack_spell_params_t*) custom_params;
    printf("\nAttack with spell");
    printf("\n%d", params->test_number);
    return 0;
}

int attack_weapon(fight_context_t * fight_context, void * custom_params) {
    attack_weapon_params * params = (attack_spell_params_t*) custom_params;
    printf("\nAttack with weapon");
    return 0;
}
