#ifndef DOOM_DEPTH_C_INVENTORY_SCREEN
#define DOOM_DEPTH_C_INVENTORY_SCREEN

#include "../sdl_utils/sdl_utils.h"

typedef enum {
    CATEGORY,
    ITEM,
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
    USE,
    THROW_AWAY
} action_options;

int display_inventory(game_window_t *game_window, section_options active_section, category_options active_category, action_options active_action);

int inventory_screen(game_window_t *game_window);

#endif