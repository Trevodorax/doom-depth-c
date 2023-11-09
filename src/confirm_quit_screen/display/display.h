#ifndef DOOM_DEPTH_C_CONFIRM_QUIT_DISPLAY_H
#define DOOM_DEPTH_C_CONFIRM_QUIT_DISPLAY_H

#include "../../game_window/game_window.h"

/**
 * @brief Displays the quit confirmation screen instead of what was previously there and starts the suit confirmation event loop
 *
 * @param game_window
 * @param quit The currently selected option
 * @return QUIT_GAME or -1
 */
int display_confirm_quit(game_window_t * game_window, bool quit);

#endif //DOOM_DEPTH_C_CONFIRM_QUIT_DISPLAY_H