#ifndef DOOM_DEPTH_C_SCREENS_ENUM
#define DOOM_DEPTH_C_SCREENS_ENUM

// no value for 1 to avoid equality with EXIT_FAILURE
typedef enum {
    TRY_AGAIN,
    QUIT_GAME = 2,
    START_MENU,
    MAP_SCREEN,
    GAME_OVER,
    FIGHT_SCREEN,
    INVENTORY_SCREEN,
    NEW_GAME_SCREEN,
    LOAD_GAME_SCREEN,
    DELETE_GAME_SCREEN,
    SHOP_SCREEN,
    LEVEL_UP_SCREEN
} router_t;

#endif