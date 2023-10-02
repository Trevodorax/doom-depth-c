#include "fight_actions.h"
#include <stdio.h>

int quit(fight_context_t * fight_context) {
    printf("\nExit");
    return -1;
}

int potion(fight_context_t * fight_context) {
    printf("\nDrink potion");
    return 0;
}

int attack_spell(fight_context_t * fight_context) {
    printf("\nAttack with spell");
    return 0;
}

int attack_weapon(fight_context_t * fight_context) {
    printf("\nAttack with weapon");
    return 0;
}
