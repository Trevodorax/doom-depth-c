#ifndef DOOM_DEPTH_C_ITEMS_MGMT_TYPES_H
#define DOOM_DEPTH_C_ITEMS_MGMT_TYPES_H

typedef enum {
    CATEGORIES,
    ITEMS,
    ACTIONS
} section_options;

typedef enum {
    GO_BACK,
    WEAPONS,
    ARMORS,
    HEALTH_POTIONS,
    MANA_POTIONS
} category_options;

typedef enum {
    HEALTH,
    MANA
} potion_types;

typedef enum {
    ARMOR,
    WEAPON
} item_types;

typedef enum {
    USE,
    THROW_AWAY
} action_options;

typedef enum {
    YES,
    NO
} confirm_options;

#endif //DOOM_DEPTH_C_ITEMS_MGMT_TYPES_H