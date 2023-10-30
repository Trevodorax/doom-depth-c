
#ifndef DOOM_DEPTH_C_FIGHT_DISPLAY_GUI_H
#define DOOM_DEPTH_C_FIGHT_DISPLAY_GUI_H

#include "../../../game_window/game_window.h"
#include "../../../utils/array.h"
#include "../../../entities/monster/monster.h"
#include "../../../entities/player/player.h"
#include "../../types.h"

/**
 * @brief Displays the fight using GUI
 *
 * @param game_window
 * @param fight_context The context of the displayed fight
 * @param fight_zone The zone in which to display the fight
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_fight_gui(game_window_t * game_window, fight_context_t * fight_context, SDL_Rect fight_zone);

#endif //DOOM_DEPTH_C_GUI_H
