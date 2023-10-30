#ifndef DOOM_DEPTH_C_CLI_H
#define DOOM_DEPTH_C_CLI_H

#include "../../../../game_window/game_window.h"
#include "../../fight_menu.h"

int display_menu_cli(game_window_t *game_window, menu_t * menu, rect_t container, int selected_item_index, bool player_turn);

#endif //DOOM_DEPTH_C_CLI_H
