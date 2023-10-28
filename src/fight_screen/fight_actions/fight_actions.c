#include "fight_actions.h"
#include "../fight_utils/fight_utils.h"
#include <stdio.h>

int heal_potion(fight_context_t *fight_context, void *custom_params) {
    if(!check_and_remove_action_points(fight_context->player, 1)) {
        build_notification(fight_context, "Not enough action points !");
        return FA_NOTHING;
    }

    printf("\nDrink heal_potion : Actual heal of player : %d - HP_MAX = %d", fight_context->player->hp,
           fight_context->player->hp_max);

    unsigned int amount = heal_player(fight_context->player, 20);

    printf("\nHealed by %d HP, actual health : %d/%d", amount, fight_context->player->hp, fight_context->player->hp_max);

    return FA_NOTHING;
}

int mana_potion(fight_context_t *fight_context, void *custom_params) {
    if (!check_and_remove_action_points(fight_context->player, 1)) {
        build_notification(fight_context, "Not enough action points !");
        return FA_NOTHING;
    }

    printf("\nDrink mana_potion : Actual mana of player : %d - MANA_MAX = %d", fight_context->player->mana,
           fight_context->player->mana_max);

    heal_mana(fight_context->player, 20);

    printf("\nMana restored ! Actual mana : %d - MANA_MAX = %d", fight_context->player->mana,
           fight_context->player->mana_max);

    return FA_NOTHING;
}

int end_turn(fight_context_t *fight_context, void *custom_params) {
    printf("\nEnd turn");
    return FA_END_TURN;
}

int attack_spell(fight_context_t *fight_context, void *custom_params) {
    printf("\nAttack with spell");
    if(!check_and_remove_action_points(fight_context->player, 2)) {
        build_notification(fight_context, "Not enough action points !");
        return FA_NOTHING;
    }

    if (fight_context->player->offensive_spell == NULL) {
        build_notification(fight_context, "No offensive spell learned !");
        return FA_NOTHING;
    }
    if (fight_context->player->mana < fight_context->player->offensive_spell->cost) {
        build_notification(fight_context, "Not enough mana !");
        return FA_NOTHING;
    }

    monster_t *target = (monster_t *) custom_params;

    // Just in case
    if (!target->hp) {
        build_notification_formatted(fight_context, "%s is already dead !", target->name);
        return FA_NOTHING;
    }

    printf("\nAttack %s with spell", target->name);
    unsigned int dmg = fight_context->player->offensive_spell->amount;
    if (dmg > target->hp) {
        dmg = target->hp;
    }
    target->hp -= dmg;

    printf("\nDamages on %s : -%dHP", target->name, dmg);
    printf("\nHP left : %d", target->hp);

    if (target->hp == 0) {
        printf("\n%s is DEAD !", target->name);

        remove_node(&fight_context->monsters, (void *)&target);

        printf("\n%d monster(s) left", get_size(fight_context->monsters));

        if (get_size(fight_context->monsters) == 0) {
            printf("\nAll monsters are dead !");
            return FA_QUIT;
        }
    }

    build_notification(fight_context, "Spell casted !");

    return FA_NOTHING;
}

int defend_spell(fight_context_t *fight_context, void *custom_params) {
    printf("\nDefend with spell");
    if(!check_and_remove_action_points(fight_context->player, 2)) {
        build_notification(fight_context, "Not enough action points !");
        return FA_NOTHING;
    }
    if (fight_context->player->defensive_spell == NULL) {
        build_notification(fight_context, "No defensive spell learned !");
        return FA_NOTHING;
    }
    if (fight_context->player->mana < fight_context->player->defensive_spell->cost) {
        build_notification(fight_context, "Not enough mana !");
        return FA_NOTHING;
    }

    fight_context->player->base_defense += fight_context->player->defensive_spell->amount;
    fight_context->player->is_defending = true;

    build_notification_formatted(fight_context, "Defend Spell casted ! Added %d defense points for next turn !",
                       fight_context->player->defensive_spell->amount);

    return FA_NOTHING;
}

int heal_spell(fight_context_t *fight_context, void *custom_params) {
    printf("\nHeal with spell");
    if(!check_and_remove_action_points(fight_context->player, 2)) {
        build_notification(fight_context, "Not enough action points !");
        return FA_NOTHING;
    }
    if (fight_context->player->healing_spell == NULL) {
        build_notification(fight_context, "No heal spell learned !");
        return FA_NOTHING;
    }
    if (fight_context->player->mana < fight_context->player->healing_spell->cost) {
        build_notification(fight_context, "Not enough mana !");
        return FA_NOTHING;
    }

    unsigned int amount = heal_player(fight_context->player, fight_context->player->healing_spell->amount);
    
    build_notification_formatted(fight_context, "Heal Spell casted ! Healed %d HP !", amount);

    return FA_NOTHING;
}

int attack_weapon(fight_context_t *fight_context, void *custom_params) {
    if(!check_and_remove_action_points(fight_context->player, 1)) {
        build_notification(fight_context, "Not enough action points !");
        return FA_NOTHING;
    }
    monster_t *target = (monster_t *) custom_params;

    if (!target->hp) {
        printf("\n%s is already dead !", target->name);
        return FA_NOTHING;
    }

    printf("\nAttack %s with weapon", target->name);

    unsigned int dmg = player_attack(fight_context->player, target);

    printf("\nDamages on %s : -%dHP", target->name, dmg);
    printf("\nHP left : %d", target->hp);

    if (target->hp == 0) {
        printf("\n%s is DEAD !", target->name);

        remove_node(&fight_context->monsters, (void *)&target);
        printf("\n%d monster(s) left", get_size(fight_context->monsters));

        if (get_size(fight_context->monsters) == 0) {
            printf("\nAll monsters are dead !");
            return FA_QUIT;
        }
    }

    return FA_NOTHING;
}
