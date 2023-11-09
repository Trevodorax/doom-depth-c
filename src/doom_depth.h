#ifndef DOOM_DEPTH_C_DOOM_DEPTH_H
#define DOOM_DEPTH_C_DOOM_DEPTH_H

#include "utils/router.h"
#include "ui_utils/sdl_utils/sdl_utils.h"
#include "game_window/game_window.h"

typedef int (*doom_depth_main)();

int doom_depth();

/**
 * @brief Parses the given argument to guess the type of ui.
 *
 * @param ui_argument
 * @return The ui type
 */
ui_type_t get_ui_type(char * ui_argument);

/**
 * @brief Factory to get the right function according to ui type
 *
 * @param ui_type
 * @return The function to call
 */
doom_depth_main doom_depth_factory(ui_type_t ui_type, color_scheme_t color_scheme);

#endif //DOOM_DEPTH_C_DOOM_DEPTH_H
