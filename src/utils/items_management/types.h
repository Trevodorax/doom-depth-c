#ifndef DOOM_DEPTH_C_ITEMS_MGMT_TYPES_H
#define DOOM_DEPTH_C_ITEMS_MGMT_TYPES_H

typedef enum {
    CATEGORIES,
    ITEMS,
    ACTIONS,
    CONFIRM
} section_options_t;

typedef enum {
    GO_BACK,
    WEAPONS,
    ARMORS,
    HEALTH_POTIONS,
    MANA_POTIONS
} category_options_t;

typedef enum {
    HEALTH,
    MANA
} potion_types_t;

typedef enum {
    ARMOR,
    WEAPON
} item_types_t;

typedef enum {
    USE,
    THROW_AWAY
} action_options_t;

#endif //DOOM_DEPTH_C_ITEMS_MGMT_TYPES_H