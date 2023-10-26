#ifndef DOOM_DEPTH_C_FIGHT_DISPLAY_H
#define DOOM_DEPTH_C_FIGHT_DISPLAY_H

#include "../../game_window/game_window.h"
#include "../types.h"
#include "../fight_menu/fight_menu.h"

/**
 * @brief Prints the fight
 *
 * @param game_window
 * @param fight_context Pointer to the data bout the fight
 * @param fight_zone Pointer to the rectangle where the fight is displayed
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_fight(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone);

#endif //DOOM_DEPTH_C_FIGHT_DISPLAY_H
