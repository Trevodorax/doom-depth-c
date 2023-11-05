#include "map_screen.h"
#include "map/map.h"
#include "map/display/display.h"
#include "stage/display/display.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "../fight_screen/fight_screen.h"

/**
 * @brief Moves the player and returns the stage he is on
 *
 * @param player_stage The stage the player is currently on
 * @param direction In which direction the player desires to move
 * @sideeffects Modifies the graph to make everything start from the player position
 * @return The stage the player is on after the move
 */
stage_t * move_player(stage_t * player_stage, orientation_t direction);

int map_screen(game_window_t * game_window, map_t * map, player_t * player) {
    stage_t * player_stage = get_player_stage(map->first_stage);

    event_t event;
    bool quit = false;
    while (!quit){
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }
        delay(game_window->ui_type, 100);

        while (get_event(game_window->ui_type, &event)) {
            switch (event) {
                case QUIT:
                    quit = true;
                    break;
                case Z_KEY:
                    player_stage = move_player(player_stage, NORTH);
                    break;
                case D_KEY:
                    player_stage = move_player(player_stage, EAST);
                    break;
                case S_KEY:
                    player_stage = move_player(player_stage, SOUTH);
                    break;
                case Q_KEY:
                    player_stage = move_player(player_stage, WEST);
                    break;
                case ENTER_KEY:
                    switch(player_stage->type) {
                        case FIGHT: {
                            if (player_stage->is_done) {
                                break;
                            }

                            router_t fight_result = fight_screen(game_window, player, player_stage->fight);

                            switch (fight_result) {
                                case MAP_SCREEN:
                                    player_stage->is_done = true;
                                    break;
                                case QUIT_GAME:
                                    // TODO: save and quit
                                case GAME_OVER:
                                default:
                                    break;
                            }

                            return fight_result;
                        }
                        case SHOP:
                            // TODO: open shop
                            break;
                        case TREASURE:
                            if (player_stage->is_done) {
                                break;
                            }
                            player_stage->is_done = true;
                            give_treasure_to_player(player_stage->treasure, player);
                            break;
                        case EMPTY:
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        map->first_stage = player_stage;
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }

        display_map(game_window, map);
        render_present(game_window);

    }

    return QUIT_GAME;
}

stage_t * move_player(stage_t * player_stage, orientation_t direction) {
    // safeguards
    if (!player_stage) {
        return NULL;
    }

    // get next stage
    stage_t *next_stage = NULL;
    switch (direction) {
        case NORTH:
            next_stage = player_stage->top;
            break;
        case EAST:
            next_stage = player_stage->right;
            break;
        case SOUTH:
            next_stage = player_stage->bottom;
            break;
        case WEST:
            next_stage = player_stage->left;
            break;
    }
    if (!next_stage) {
        player_stage->player_orientation = direction;
        return player_stage;
    }

    // prevent going to the next fight if
    if(player_stage->fight && !player_stage->is_done && !next_stage->is_done) {
        return player_stage;
    }

    // switch player to next stage
    next_stage->player = player_stage->player;
    player_stage->player = NULL;
    next_stage->player_orientation = direction;
    player_stage->player_orientation = 0;

    switch (direction) {
        case NORTH:
            next_stage->bottom = player_stage;
            player_stage->top = NULL;
            break;
        case EAST:
            next_stage->left = player_stage;
            player_stage->right = NULL;
            break;
        case SOUTH:
            next_stage->top = player_stage;
            player_stage->bottom = NULL;
            break;
        case WEST:
            next_stage->right = player_stage;
            player_stage->left = NULL;
            break;
    }

    if(player_stage->type != FIGHT && player_stage->type != TREASURE) {
        player_stage->is_done = true;
    }

    return next_stage;
}
