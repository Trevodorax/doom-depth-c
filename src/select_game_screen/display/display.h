#ifndef DOOM_DEPTH_C_SELECT_GAME_SCREEN_DISPLAY_H
#define DOOM_DEPTH_C_SELECT_GAME_SCREEN_DISPLAY_H

#include "../../game_window/game_window.h"
#include "../../ui_utils/sdl_utils/sdl_utils.h"
#include "../../ui_utils/cli_utils/cli_utils.h"
#include "../../ui_utils/ui_utils.h"
#include "../../utils/array.h"
#include "../../entities/player/player.h"

/**
 * @brief Displays the load game screen.
 *
 * This function displays the load game screen.
 *
 * @param game_window A pointer to the game window.
 * @param players A pointer to the array of players.
 * @param active_option The index of the active option.
 *
 * @return EXIT_SUCCESS.
 * @sideeffects May modify the SDL renderer by rendering new elements.
 * @dependencies Depends on the SDL2 library.
 * @errors None.
 */
int display_select_game(game_window_t *game_window, array_node_t *players, unsigned short active_option, char * title);

/**
 * @brief Displays the load game screen GUI.
 *
 * This function displays the load game screen GUI.
 *
 * @param game_window A pointer to the game window.
 * @param players A pointer to the array of players.
 * @param active_option The index of the active option.
 * @param title The title of the screen.
 *
 * @return EXIT_SUCCESS.
 * @sideeffects May modify the SDL renderer by rendering new elements.
 * @dependencies Depends on the SDL2 library.
 * @errors None.
 */
int display_select_game_gui(game_window_t *game_window, array_node_t *players, unsigned short active_option, char * title);

/**
 * @brief Displays the load game screen CLI.
 *
 * This function displays the load game screen CLI.
 *
 * @param game_window A pointer to the game window.
 * @param players A pointer to the array of players.
 * @param active_option The index of the active option.
 * @param title The title of the screen.
 *
 * @return EXIT_SUCCESS.
 * @sideeffects May modify the terminal by printing new elements.
 * @dependencies None.
 * @errors None.
 */
int display_select_game_cli(game_window_t *game_window, array_node_t *players, unsigned short active_option, char * title);

#endif //DOOM_DEPTH_C_SELECT_GAME_SCREEN_DISPLAY_H