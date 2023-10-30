#ifndef DOOM_DEPTH_C_FIGHT_DISPLAY_CLI_H
#define DOOM_DEPTH_C_FIGHT_DISPLAY_CLI_H

#include "../../../game_window/game_window.h"
#include "../../types.h"

/**
 * @brief Displays the fight using cli
 *
 * @param game_window
 * @param fight_context The context of the displayed fight
 * @param fight_zone The zone in which to display the fight
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_fight_cli(game_window_t * game_window, fight_context_t * fight_context, rect_t fight_zone);

#endif //DOOM_DEPTH_C_FIGHT_DISPLAY_CLI_H
