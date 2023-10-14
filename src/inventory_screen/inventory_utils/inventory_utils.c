#include <stdlib.h>
#include "string.h"
#include "inventory_utils.h"
#include "../../utils/utils.h"

char *armor_details_to_string(armor_t *armor) {
    char buffer[4];

    char *res = malloc(sizeof(char) * 1000);
    res[0] = '\0';

    strcat(res, "Name : ");
    strcat(res, custom_strupr(armor->name));
    strcat(res, "\n");

    strcat(res, "Rarity : ");
    strcat(res, custom_itoa((int) armor->rarity, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Defense value : ");
    strcat(res, custom_itoa((int) armor->amount, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Uses left : ");
    strcat(res, custom_itoa((int) armor->uses, buffer, 10));
    strcat(res, " out of ");
    strcat(res, custom_itoa((int) armor->max_uses, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Cost : ");
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

    strcat(res, "Name : ");
    strcat(res, custom_strupr(weapon->name));
    strcat(res, "\n");

    strcat(res, "Rarity : ");
    strcat(res, custom_itoa((int) weapon->rarity, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Attacks per turn : ");
    strcat(res, custom_itoa((int) weapon->attacks_per_turn, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Attack range : ");
    strcat(res, custom_itoa((int) weapon->min_attack, buffer, 10));
    strcat(res, " - ");
    strcat(res, custom_itoa((int) weapon->max_attack, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Uses left : ");
    strcat(res, custom_itoa((int) weapon->uses, buffer, 10));
    strcat(res, " out of ");
    strcat(res, custom_itoa((int) weapon->max_uses, buffer, 10));
    strcat(res, "\n");

    strcat(res, "Cost : ");
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

    strcat(res, "Name : HEALTH POTION\n");
    strcat(res, "Healing power : 20\n");
    strcat(res, "In stock : ");
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

    strcat(res, "Name : MANA POTION\n");
    strcat(res, "Mana value : 20\n");
    strcat(res, "In stock : ");
    strcat(res, custom_itoa((int) quantity, buffer, 10));

    res = realloc(res, sizeof(char) * (strlen(res) + 1));
    if (!res) {
        return NULL;
    }

    return res;
}