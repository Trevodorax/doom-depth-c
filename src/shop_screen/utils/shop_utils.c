#include "shop_utils.h"
#include "../../utils/array.h"

int get_items_count(category_options category) {
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