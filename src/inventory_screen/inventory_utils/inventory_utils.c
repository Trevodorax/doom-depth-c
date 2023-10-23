#include <stdlib.h>
#include "string.h"
#include "inventory_utils.h"
#include "../../utils/utils.h"

char *armor_details_to_string(armor_t *armor) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name :\n");
    strcat(res, custom_strupr(armor->name));
    strcat(res, "\n\n");

    strcat(res, "Rarity :\n");
    strcat(res, custom_itoa((int) armor->rarity, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Defense value :\n");
    strcat(res, custom_itoa((int) armor->amount, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Uses left :\n");
    strcat(res, custom_itoa((int) armor->uses, buffer, 10));
    strcat(res, " out of ");
    strcat(res, custom_itoa((int) armor->max_uses, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Cost :\n");
    strcat(res, custom_itoa((int) armor->cost, buffer, 10));

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}

char *weapon_details_to_string(weapon_t *weapon) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name :\n");
    strcat(res, custom_strupr(weapon->name));
    strcat(res, "\n\n");

    strcat(res, "Rarity :\n");
    strcat(res, custom_itoa((int) weapon->rarity, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Attacks per turn :\n");
    strcat(res, custom_itoa((int) weapon->attacks_per_turn, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Attack range :\n");
    strcat(res, custom_itoa((int) weapon->min_attack, buffer, 10));
    strcat(res, " - ");
    strcat(res, custom_itoa((int) weapon->max_attack, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Uses left :\n");
    strcat(res, custom_itoa((int) weapon->uses, buffer, 10));
    strcat(res, " out of ");
    strcat(res, custom_itoa((int) weapon->max_uses, buffer, 10));
    strcat(res, "\n\n");

    strcat(res, "Cost :\n");
    strcat(res, custom_itoa((int) weapon->cost, buffer, 10));

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}

char *health_potions_details_to_string(unsigned int quantity) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name :\nHEALTH POTION\n\n");
    strcat(res, "Healing power :\n20\n\n");
    strcat(res, "In stock :\n");
    strcat(res, custom_itoa((int) quantity, buffer, 10));

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}

char *mana_potions_details_to_string(unsigned int quantity) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name :\nMANA POTION\n\n");
    strcat(res, "Mana value :\n20\n\n");
    strcat(res, "In stock :\n");
    strcat(res, custom_itoa((int) quantity, buffer, 10));

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}

bool is_full(inventory_t *inventory) {
    int items_count = inventory->nb_armors + inventory->nb_weapons + inventory->nb_health_potions + inventory->nb_mana_potions;
    return inventory->capacity > items_count;
}