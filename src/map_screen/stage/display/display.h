#ifndef DOOM_DEPTH_C_STAGE_DISPLAY_H
#define DOOM_DEPTH_C_STAGE_DISPLAY_H

#include "../../../game_window/game_window.h"
#include "../stage.h"

/**
 * @brief Gets the list of textures for each stage type
 *
 * @param renderer The renderer for which to create the textures
 * @return The list of textures in the order of the stages enum
 */
SDL_Texture ** get_stage_textures(SDL_Renderer * renderer);

/**
 * @brief Uncounts and displays the given stage and its stages recursively
 *
 * @param game_window The window to print the stages on
 * @param stages The stage to be displayed
 * @param x_coord The x-coord of the stage
 * @param y_coord The y-coord of the stage
 * @param stage_size The size of the stage
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int print_stages(game_window_t *game_window, stage_t *stages, SDL_Texture **stage_textures, int x_coord, int y_coord,
                 int stage_size, bool with_players);


/**
 * @brief Displays the given stage only
 *
 * @param game_window The window to print the stages on
 * @param stage The stage to be displayed
 * @param x_coord The x-coord of the stage
 * @param y_coord The y-coord of the stage
 * @param stage_size The size of the stage
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int print_stage(game_window_t *game_window, stage_t *stage, SDL_Texture **stage_textures, int x_coord, int y_coord, int stage_size, bool with_players);

#endif //DOOM_DEPTH_C_STAGE_DISPLAY_H
