#ifndef DOOM_DEPTH_C_GUI_H
#define DOOM_DEPTH_C_GUI_H

#include "../../../../game_window/game_window.h"
#include "../../fight_menu.h"

int display_menu_gui(game_window_t *game_window, menu_t * menu, rect_t container, int selected_item_index, bool player_turn);

#endif //DOOM_DEPTH_C_GUI_H
