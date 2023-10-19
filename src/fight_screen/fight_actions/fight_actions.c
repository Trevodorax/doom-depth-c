#include "fight_actions.h"
#include <stdio.h>

int quit(fight_context_t * fight_context, void * custom_params) {
    printf("\nExit");
    return FA_QUIT;
}

int heal_potion(fight_context_t * fight_context, void * custom_params) {
    if(fight_context->player->action_points < 1){
        build_notification(fight_context,"Not enough action points !");
        return FA_POTION;
    }
    printf("\nDrink heal_potion : Actual heal of player : %d - HP_MAX = %d",fight_context->player->hp,fight_context->player->hp_max);
    fight_context->player->heal(fight_context->player,20);
    printf("\nHealed ! Actual health : %d - HP_MAX = %d", fight_context->player->hp,fight_context->player->hp_max);
    fight_context->player->action_points -= 1;
    return FA_POTION;
}

int mana_potion(fight_context_t * fight_context, void * custom_params) {
    if(fight_context->player->action_points < 1){
        build_notification(fight_context,"Not enough action points !");
        return FA_POTION;
    }
    printf("\nDrink mana_potion : Actual mana of player : %d - MANA_MAX = %d",fight_context->player->mana,fight_context->player->mana_max);
    fight_context->player->mana += 20;
    if(fight_context->player->mana > fight_context->player->mana_max){
        fight_context->player->mana = fight_context->player->mana_max;
    }
    printf("\nMana restored ! Actual mana : %d - MANA_MAX = %d", fight_context->player->mana,fight_context->player->mana_max);
    fight_context->player->action_points -= 1;
    return FA_POTION;
}

int end_turn(fight_context_t * fight_context, void * custom_params){
    printf("\nEnd turn");
    fight_context->player_turn = false;
    return 99;
}

int attack_spell(fight_context_t * fight_context, void * custom_params) {
    printf("\nAttack with spell");
    if(fight_context->player->action_points < 2){
        build_notification(fight_context,"Not enough action points !");
        return FA_SPELL;
    }
    monster_t * target = (monster_t*) custom_params;
    if(!target->hp) {
        build_notification_formatted(fight_context,"%s is already dead !",target->name);
        return FA_SPELL;
    }
    printf("\nAttack %s with spell",target->name);

    unsigned int dmg = fight_context->player->offensive_spell->amount;
    if(dmg > target->hp){
        dmg = target->hp;
    }
    target->hp -= dmg;
    printf("\nDamages on %s : -%dHP",target->name,dmg);
    printf("\nHP left : %d",target->hp);
    if(target->hp == 0){
        printf("\n%s is DEAD !",target->name);
        remove_node(&fight_context->monsters,&target);
        printf("\n%d monster(s) left",get_size(fight_context->monsters));
        if(get_size(fight_context->monsters) == 0){
            printf("\nAll monsters are dead !");
            return FA_QUIT;
        }
    }
    fight_context->player->action_points -= 2;

    build_notification(fight_context,"Spell casted !");
    return FA_SPELL;
}

int attack_weapon(fight_context_t * fight_context, void * custom_params) {
    monster_t * target = (monster_t*) custom_params;
    if(fight_context->player->action_points < 1){
        build_notification(fight_context,"Not enough action points !");
        return FA_WEAPON;
    }
    if(!target->hp) {
        printf("\n%s is already dead !",target->name);

        return FA_WEAPON;
    }
    printf("\nAttack %s with weapon",target->name);

    int dmg = fight_context->player->attack(fight_context->player,target);

    printf("\nDamages on %s : -%dHP",target->name,dmg);
    printf("\nHP left : %d",target->hp);
    if(target->hp == 0){
        printf("\n%s is DEAD !",target->name);
        remove_node(&fight_context->monsters,&target);
        printf("\n%d monster(s) left",get_size(fight_context->monsters));
        if(get_size(fight_context->monsters) == 0){
            printf("\nAll monsters are dead !");
            return FA_QUIT;
        }
    }
    fight_context->player->action_points -= 1;
    return FA_WEAPON;
}
