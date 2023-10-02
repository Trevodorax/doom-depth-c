#ifndef DOOM_DEPTH_C_SCREENS_ENUM
#define DOOM_DEPTH_C_SCREENS_ENUM

// no value for 1 to avoid equality with EXIT_FAILURE
typedef enum {
    TRY_AGAIN,
    QUIT = 2,
    START_MENU,
    MAP,
    GAME_OVER
} Router;

#endif