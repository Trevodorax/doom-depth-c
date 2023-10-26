#ifndef DOOM_DEPTH_C_FIGHT_MENU_DISPLAY_H
#define DOOM_DEPTH_C_FIGHT_MENU_DISPLAY_H

#include <SDL2/SDL.h>
#include "../fight_menu.h"

/**
 * @brief Prints one level of menu
 *
 * @param game_window
 * @param menu The printed menu
 * @param container The rectangle in which to print the menu
 * @param selected_item_index The index of the selected item
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_menu(game_window_t *game_window, menu_t * menu, rect_t container, int selected_item_index, bool player_turn);

#endif //DOOM_DEPTH_C_FIGHT_MENU_DISPLAY_H
