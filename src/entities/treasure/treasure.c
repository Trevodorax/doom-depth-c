#include "treasure.h"
#include "../../logs/log.h"

int give_treasure_to_player(treasure_t * treasure, player_t * player) {
    if(!treasure || !player) {
        return EXIT_FAILURE;
    }

    player->gold += treasure->coins;

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
    json_coins->number = (int)treasure->coins;
    add_key_value_to_object(&object, "coins", json_coins);
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

    return result;
}
