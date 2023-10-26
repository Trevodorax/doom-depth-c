#include "shop_utils.h"
#include "../../utils/array.h"
#include "../../utils/utils.h"
#include <stdlib.h>

int get_items_count(category_options_t category) {
    switch (category) {
        case ARMORS: {
            array_node_t *armors = get_armors();
            return get_count(armors);
        }

        case WEAPONS: {
            array_node_t *weapons = get_weapons();
            return get_count(weapons);
        }

        case HEALTH_POTIONS:
        case MANA_POTIONS:
            return 2;

        case GO_BACK:
        default:
            return -1;
    }
}

char * player_gold_to_string(player_t * player) {
    char *res = malloc(sizeof(char) * 10);
    res[0] = '\0';
    custom_itoa((int) player->gold, res, 10);
    return res;
}

char * shop_armor_details_to_string(armor_t * armor) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name : ");
    strcat(res, custom_strupr(armor->name));
    strcat(res, "\n\n");

    strcat(res, "Rarity : ");
    strcat(res, custom_itoa((int) armor->rarity, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Defense value : ");
    strcat(res, custom_itoa((int) armor->amount, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Uses : ");
    strcat(res, custom_itoa((int) armor->max_uses, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Cost : ");
    strcat(res, custom_itoa((int) armor->cost, buffer, 10));
    strcat(res,"\n");

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}

char *shop_weapon_details_to_string(weapon_t *weapon) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name : ");
    strcat(res, custom_strupr(weapon->name));
    strcat(res, "\n\n");

    strcat(res, "Rarity : ");
    strcat(res, custom_itoa((int) weapon->rarity, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Attacks per turn : ");
    strcat(res, custom_itoa((int) weapon->attacks_per_turn, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Attack range : ");
    strcat(res, custom_itoa((int) weapon->min_attack, buffer, 10));
    strcat(res, " - ");
    strcat(res, custom_itoa((int) weapon->max_attack, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Uses : ");
    strcat(res, custom_itoa((int) weapon->max_uses, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Cost : ");
    strcat(res, custom_itoa((int) weapon->cost, buffer, 10));
    strcat(res,"\n");

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}

char *shop_health_potions_details_to_string() {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name : HEALTH POTION\n\n");
    strcat(res, "Healing power : 20\n\n");
    strcat(res, "Cost : ");
    strcat(res, custom_itoa(HEALTH_POTIONS_COST, buffer, 10));
    strcat(res,"\n");

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}

char *shop_mana_potions_details_to_string() {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name : MANA POTION\n\n");
    strcat(res, "Mana value : 20\n\n");
    strcat(res, "Cost : ");
    strcat(res, custom_itoa(MANA_POTIONS_COST, buffer, 10));
    strcat(res,"\n");

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}