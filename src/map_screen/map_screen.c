#include "map_screen.h"
#include "map/map.h"
#include "map/display/display.h"
#include "stage/display/display.h"
#include "../event/event.h"
#include "../utils/utils.h"
#include "../fight_screen/fight_screen.h"
#include "../fight_screen/fight_utils/fight_utils.h"
#include "../inventory_screen/inventory_screen.h"
#include "../help_screen/help_screen.h"
#include "../confirm_quit_screen/confirm_quit_screen.h"
#include "../stats_screen/stats_screen.h"
#include "../map_generation/map_generation.h"

/**
 * @brief Moves the player and returns the stage he is on
 *
 * @param player_stage The stage the player is currently on
 * @param direction In which direction the player desires to move
 * @sideeffects Modifies the graph to make everything start from the player position
 * @return The stage the player is on after the move
 */
stage_t * move_player(stage_t * player_stage, orientation_t direction);

int map_screen(game_window_t * game_window, map_t ** map, player_t * player) {
    stage_t * player_stage = get_player_stage((*map)->first_stage);

    if (player_stage->fight_context) {
        player_stage->fight_context->player = player;
        router_t fight_result = fight_screen(game_window, player, player_stage, player_stage->fight_context);
        switch (fight_result) {
            case MAP_SCREEN:
                player_stage->is_done = true;
                player_stage->fight_context = NULL;
                break;
            default:
                break;
        }

        return fight_result;
    }

    event_t event;
    bool quit = false;
    while (!quit){
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(true);
        }
        delay(game_window->ui_type, 100);

        while (get_event(game_window->ui_type, &event)) {
            switch (event) {
                case Q_KEY:
                case QUIT:
                    if (confirm_quit_screen(game_window) == QUIT_GAME) {
                        return QUIT_GAME;
                    }
                    break;
                case z_KEY:
                    player_stage = move_player(player_stage, NORTH);
                    break;
                case d_KEY:
                    player_stage = move_player(player_stage, EAST);
                    break;
                case s_KEY:
                    player_stage = move_player(player_stage, SOUTH);
                    break;
                case q_KEY:
                    player_stage = move_player(player_stage, WEST);
                    break;
                case i_KEY:
                    if(inventory_screen(game_window, player) == EXIT_FAILURE) {
                        return EXIT_FAILURE;
                    }
                    break;
                case m_KEY:
                    stats_screen(game_window, player);
                    break;
                case h_KEY:
                    help_screen(game_window);
                    break;
                case ENTER_KEY:
                    switch(player_stage->type) {
                        case FIGHT: {
                            if (player_stage->is_done) {
                                break;
                            }

                            router_t fight_result = fight_screen(game_window, player, player_stage, NULL);

                            switch (fight_result) {
                                case MAP_SCREEN:
                                    player_stage->is_done = true;
                                    break;
                                case QUIT_GAME:
                                case GAME_OVER:
                                default:
                                    break;
                            }

                            return fight_result;
                        }
                        case SHOP:
                            return SHOP_SCREEN;
                        case TREASURE:
                            if (player_stage->is_done) {
                                break;
                            }
                            player_stage->is_done = true;
                            give_treasure_to_player(player_stage->treasure, player);
                            break;
                        case LINKED_MAP:
                            save_player_map(player, *map);
                            // map that wasn't already generated
                            if (strlen(player_stage->linked_map_file_path) == 0) {
                                map_t * generated_map = generate_map(*map);
                                save_map("../assets/maps", generated_map);
                                free(player_stage->linked_map_file_path);
                                player_stage->linked_map_file_path = strdup(generated_map->name);
                            }
                            player->current_map = player_stage->linked_map_file_path;
                            *map = get_player_map(player);
                            return MAP_SCREEN;
                        case EMPTY:
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        (*map)->first_stage = player_stage;
        if (game_window->ui_type == CLI) {
            set_cli_raw_mode(false);
        }

        display_map(game_window, *map);
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
