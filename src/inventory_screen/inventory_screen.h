#ifndef DOOM_DEPTH_C_INVENTORY_SCREEN
#define DOOM_DEPTH_C_INVENTORY_SCREEN

#include "../sdl_utils/sdl_utils.h"
#include "../entities/player/player.h"

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

typedef enum {
    HEALTH,
    MANA
} potion_types;

typedef enum {
    ARMOR,
    WEAPON
} item_types;

/**
 * @brief switches the window to inventory screen
 *
 * It starts the inventory event loop, and prints the inventory choices on the screen instead of what was previously there
 *
 * @param game_window the window to display the inventory on
 * @param player the data of the player currently playing
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int inventory_screen(game_window_t *game_window, player_t *player);

#endif