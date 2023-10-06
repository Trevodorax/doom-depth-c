#ifndef DOOM_DEPTH_C_DOOM_DEPTH_H
#define DOOM_DEPTH_C_DOOM_DEPTH_H

typedef enum {
    CLI,
    GUI
} ui_type_t;

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
doom_depth_main doom_depth_factory(ui_type_t ui_type);

#endif //DOOM_DEPTH_C_DOOM_DEPTH_H
