#ifndef DOOM_DEPTH_C_GAME_WINDOW_H
#define DOOM_DEPTH_C_GAME_WINDOW_H

#include <SDL2/SDL.h>
#include "../cli_utils/cli_utils.h"
#include "../utils/router.h"

typedef enum {
    CLI,
    GUI
} ui_type_t;

typedef struct {
    Router current_screen;
} window_context_t;

typedef struct {
    ui_type_t ui_type;

    // for GUI mode
    SDL_Window * window;
    SDL_Renderer * renderer;

    // for CLI mode
    cli_matrix_t * matrix;

    window_context_t * context;
} game_window_t;

/**
 * @brief Frees completely a game window
 *
 * Cleans up and frees the resources that are associated with
 * a game window, including the window, renderer, and context
 *
 * @param game_window Pointer to the game window structure to be freed
 */
void free_game_window(game_window_t *game_window, ui_type_t ui_type);
void free_game_window_cli(game_window_t * game_window);
void free_game_window_gui(game_window_t *game_window);

/**
 * @brief Initializes a new game window
 *
 * This function allocates and initializes a new game window structure,
 * initializing the right parts for the requested ui_type
 *
 * @param ui_type
 * @return Pointer to the newly created game window structure
 */
game_window_t *init_game_window(ui_type_t ui_type);
int init_game_window_cli(game_window_t * game_window);
int init_game_window_gui(game_window_t * game_window);

/**
 * @brief Renders the changes made to the UI
 *
 * @param game_window
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int render_present(game_window_t * game_window);

#endif //DOOM_DEPTH_C_GAME_WINDOW_H
