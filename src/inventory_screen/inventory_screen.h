#ifndef DOOM_DEPTH_C_INVENTORY_SCREEN
#define DOOM_DEPTH_C_INVENTORY_SCREEN

#include "../sdl_utils/sdl_utils.h"

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
    USE,
    THROW_AWAY
} action_options;

int display_inventory(game_window_t *game_window, inventory_t *inventory, section_options active_section, category_options active_category, action_options active_action, unsigned short active_item);

int inventory_screen(game_window_t *game_window, player_t *player);

#endif