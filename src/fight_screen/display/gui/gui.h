
#ifndef DOOM_DEPTH_C_FIGHT_DISPLAY_GUI_H
#define DOOM_DEPTH_C_FIGHT_DISPLAY_GUI_H

#include "../../../game_window/game_window.h"
#include "../../../utils/array.h"
#include "../../../entities/monster/monster.h"
#include "../../../entities/player/player.h"
#include "../../types.h"

int display_fight_gui(game_window_t * game_window, fight_context_t * fight_context, SDL_Rect fight_zone);
int display_monsters_gui(game_window_t * game_window, array_node_t * monsters, rect_t container);
int display_monster_gui(game_window_t * game_window, monster_t *monster, SDL_Rect container);
int display_player_in_fight_gui(game_window_t * game_window, player_t * player, SDL_Rect container);
int display_notification_zone_gui(game_window_t * game_window, char * notification_text, SDL_Rect container);
int display_stat_bar_gui(game_window_t *game_window, int current_stat, int max_stat, SDL_Rect container,
                         SDL_Color filled_color, SDL_Color empty_color);
int display_player_stats_zone_gui(game_window_t * game_window, player_t * player, SDL_Rect container);

#endif //DOOM_DEPTH_C_GUI_H
