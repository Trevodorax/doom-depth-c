#include "treasure.h"
#include "../../logs/log.h"

int give_treasure_to_player(treasure_t * treasure, player_t * player) {
    if(!treasure || !player) {
        return EXIT_FAILURE;
    }

    player->gold += treasure->coins;

    if(player->inventory) {
        if(treasure->armor) {
            if(player->inventory->armors_head == NULL) {
                player->inventory->armors_head = malloc(sizeof(array_node_t));
                player->inventory->armors_head->value = treasure->armor;
                player->inventory->armors_head->next = NULL;
            } else {
                push(&(player->inventory->armors_head), treasure->armor, sizeof(armor_t));
            }
            player->inventory->nb_armors++;
        }

        if(treasure->weapon) {
            if(player->inventory->weapons_head == NULL) {
                player->inventory->weapons_head = malloc(sizeof(array_node_t));
                player->inventory->weapons_head->value = treasure->weapon;
                player->inventory->weapons_head->next = NULL;
            } else {
                push(&(player->inventory->weapons_head), treasure->weapon, sizeof(weapon_t));
            }
            player->inventory->nb_weapons++;
        }

        player->inventory->nb_mana_potions += treasure->mana_potions;
    }

    return EXIT_SUCCESS;
}

void add_treasure_to_json_object(json_t * object, treasure_t * treasure) {
    if (!treasure || !object || object->type != 'o') {
        global_logger->error("\ntreasure_to_json error: wrong parameters");
        return;
    }

    // coins
    json_t * json_coins = malloc(sizeof(json_t));
    json_coins->type = 'n';
    json_coins->number = treasure->coins;

    // armor
    json_t * json_armor = malloc(sizeof(json_t));
    json_armor->type = 'n';
    if (treasure->armor) {
        json_armor->number = treasure->armor->id;
    } else {
        json_armor->number = 0;
    }

    // weapon
    json_t * json_weapon = malloc(sizeof(json_t));
    json_weapon->type = 'n';
    if (treasure->weapon) {
        json_weapon->number = treasure->weapon->id;
    } else {
        json_weapon->number = 0;
    }

    // mana_potions
    json_t * json_mana_potions = malloc(sizeof(json_t));
    json_mana_potions->type = 'n';
    json_mana_potions->number = treasure->mana_potions;

    add_key_value_to_object(&object, "coins", json_coins);
    add_key_value_to_object(&object, "mana_potions", json_mana_potions);
    add_key_value_to_object(&object, "armor", json_armor);
    add_key_value_to_object(&object, "weapon", json_weapon);
}

treasure_t * json_to_treasure(json_t * treasure_json) {
    if (!treasure_json || treasure_json->type != 'o') {
        global_logger->error("\njson_to_treasure error: invalid input json\n");
        return NULL;
    }

    treasure_t *result = malloc(sizeof(treasure_t));
    if (!result) {
        global_logger->error("\njson_to_treasure error: memory allocation failed\n");
        return NULL;
    }

    // coins
    json_t * coins = get_object_at_key(treasure_json, "coins");
    if (!coins || coins->type != 'n' || coins->number == 0) {
        free(result);
        return NULL;
    }
    result->coins = coins->number;

    // armor
    json_t * armor = get_object_at_key(treasure_json, "armor");
    if (!armor || armor->type != 'n' || armor->number <= 0) {
        result->armor = NULL;
    } else {
        result->armor = get_armor_by_id(armor->number);
    }

    // weapon
    json_t * weapon = get_object_at_key(treasure_json, "weapon");
    if (!weapon || weapon->type != 'n' || weapon->number <= 0) {
        result->weapon = NULL;
    } else {
        result->weapon = get_weapon_by_id(weapon->number);
    }

    // mana_potions
    json_t * mana_potions = get_object_at_key(treasure_json, "mana_potions");
    if (!mana_potions || mana_potions->type != 'n' || mana_potions->number <= 0) {
        result->mana_potions = 0;
    } else {
        result->mana_potions = mana_potions->number;
    }

    return result;
}
