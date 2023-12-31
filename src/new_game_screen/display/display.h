#ifndef DOOM_DEPTH_C_NEW_GAME_SCREEN_DISPLAY_H
#define DOOM_DEPTH_C_NEW_GAME_SCREEN_DISPLAY_H

#include "../../game_window/game_window.h"

/**
 * @brief displays the new game screen
 *
 * @param game_window the window to display the map on
 * @param name the name of the player
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_new_game(game_window_t *game_window, char *name);

/**
 * @brief handles the different parts of the new game screen CLI and their display
 *
 * @param game_window the window to display the map on
 * @param name the name of the player
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_new_game_cli(game_window_t *game_window, char *name);

/**
 * @brief handles the different parts of the new game screen GUI and their display
 *
 * @param game_window the window to display the map on
 * @param name the name of the player
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_new_game_gui(game_window_t *game_window, char *name);

/**
 * @brief displays a welcome message : "Welcome new player"
 *
 * @param game_window the game window to use
 * @param container the rectangle in which the message will be displayed
 * @param window_width the width of the window in which to display the message
 * @param window_height the height of the window in which to display the message
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_welcome(game_window_t * game_window, SDL_Rect *container, int window_width, int window_height);

/**
 * @brief displays a question to ask the player for their name : "What is your name ?"
 *
 * @param game_window the game window to use
 * @param container the rectangle in which the message will be displayed
 * @param width_ref the reference coordinate for the width of the message, the message will be displayed in the middle of the ref
 * @param height_ref the reference coordinate for the height of the message, the message will be displayed 64 pixels under the ref
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_name_question(game_window_t * game_window, SDL_Rect *container, int width_ref, int height_ref);

/**
 * @brief displays an input for the player to enter their name
 *
 * @param game_window the game window to use
 * @param container the rectangle in which the input will be displayed
 * @param width_ref the reference coordinate for the width of the input, the message will be displayed in the middle of the ref
 * @param height_ref the reference coordinate for the height of the input, the message will be displayed 64 pixels under the ref
 * @param name the value of the input, aka the name entered by the player
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_name_input(game_window_t * game_window, SDL_Rect *container, int width_ref, int height_ref, const char *name);

/**
 * @brief displays a cursor
 *
 * @param game_window the game window to use
 * @param container the rectangle in which the message will be displayed
 * @param width_ref the reference coordinate for the width of the message, the message will be displayed at the ref
 * @param height_ref the reference coordinate for the height of the message, the message will be displayed at the ref
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_name_input_cursor(game_window_t * game_window, SDL_Rect *container, int width_ref, int height_ref);

#endif