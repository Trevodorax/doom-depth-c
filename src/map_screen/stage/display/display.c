#include "display.h"

#include "../../../ui_utils/sdl_utils/sdl_utils.h"

int print_stages_rec(game_window_t *game_window, stage_t *stages, int x_coord, int y_coord, int stage_size, bool with_players);

/**
 * @brief Prints a player somewhere on the map
 *
 * Draws the player image orientated in the player orientation.
 *
 * @param game_window The game window
 * @param player The player to be displayed (ignored by the current implementation)
 * @param player_orientation The orientation of the player on the map
 * @param stage_rect The rectangle to print the player in
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int print_player_on_stage_gui(game_window_t * game_window, orientation_t player_orientation, SDL_Rect stage_rect);
int print_player_on_stage_cli(game_window_t * game_window, orientation_t player_orientation, rect_t stage_rect);

SDL_Texture ** get_stage_textures(SDL_Renderer * renderer) {
    static SDL_Texture ** stage_textures = NULL;
    if (stage_textures) {
        return stage_textures;
    }

    stage_textures = malloc(sizeof(SDL_Texture*) * STAGE_TYPE_COUNT);
    for (int i = 0; i < STAGE_TYPE_COUNT; i++) {
        stage_textures[i] = get_image_texture(renderer, stage_texture_files[i]);
        if (!stage_textures[i]) {
            // cleanup and leave
            fprintf(stderr, "\nget_stage_textures error: failed to get the file textures.");
            for (int j = 0; j < i; j++) {
                free(stage_textures[j]);
            }
            free(stage_textures);
            return NULL;
        }
    }
    return stage_textures;
}

int print_stages(game_window_t *game_window, stage_t *stages, int x_coord, int y_coord, int stage_size, bool with_players) {
    uncount_stages(stages);
    print_stages_rec(game_window, stages, x_coord, y_coord, stage_size, with_players);

    return EXIT_SUCCESS;
}

int print_stages_rec(game_window_t *game_window, stage_t *stages, int x_coord, int y_coord, int stage_size, bool with_players) {
    if (!stages || stages->counted) {
        return EXIT_SUCCESS;
    }

    stages->counted = true;

    print_stage(game_window, stages, x_coord * stage_size, y_coord * stage_size, stage_size, with_players);

    print_stages_rec(game_window, stages->top, x_coord, y_coord - 1, stage_size, with_players);
    print_stages_rec(game_window, stages->right, x_coord + 1, y_coord, stage_size, with_players);
    print_stages_rec(game_window, stages->bottom, x_coord, y_coord + 1, stage_size, with_players);
    print_stages_rec(game_window, stages->left, x_coord - 1, y_coord, stage_size, with_players);

    return EXIT_SUCCESS;
}

int print_stage_cli(game_window_t *game_window, stage_t *stage, int x_coord, int y_coord, int stage_size, bool with_players);
int print_stage_gui(game_window_t *game_window, stage_t *stage, int x_coord, int y_coord, int stage_size, bool with_players);

int print_stage(game_window_t *game_window, stage_t *stage, int x_coord, int y_coord, int stage_size, bool with_players) {
    switch (game_window->ui_type) {
        case CLI:
            return print_stage_cli(game_window, stage, x_coord, y_coord, stage_size, with_players);
        case GUI:
            return print_stage_gui(game_window, stage, x_coord, y_coord, stage_size, with_players);
    }
}

int print_stage_gui(game_window_t *game_window, stage_t *stage, int x_coord, int y_coord, int stage_size, bool with_players) {
    SDL_Rect stage_rect =  {x_coord, y_coord, stage_size, stage_size};

    SDL_Texture ** stage_textures = get_stage_textures(game_window->renderer);
    SDL_Texture * stage_texture = NULL;
    if(stage->type == FIGHT && stage->is_done) {
        stage_texture = stage_textures[FIGHT_DONE];
    } else {
        stage_texture = stage_textures[stage->type];
    }

    if (SDL_RenderCopy(game_window->renderer, stage_texture, NULL, &stage_rect) != 0) {
        return EXIT_FAILURE;
    }

    if (stage->player && with_players) {
        print_player_on_stage_gui(game_window, stage->player_orientation, stage_rect);
    }

    return EXIT_SUCCESS;
}

int print_stage_cli(game_window_t *game_window, stage_t *stage, int x_coord, int y_coord, int stage_size, bool with_players) {
    rect_t stage_rect = {x_coord, y_coord, stage_size, stage_size};
    color_code_t stage_color = get_stage_color(stage);

    cli_draw_fill_rect(game_window->matrix, stage_rect, (cli_char_t){'#', stage_color});

    if (stage->player && with_players) {
        print_player_on_stage_cli(game_window, stage->player_orientation, stage_rect);
    }

    return EXIT_SUCCESS;
}

int print_player_on_stage_gui(game_window_t * game_window, orientation_t player_orientation, SDL_Rect stage_rect) {
    int stage_padding = stage_rect.h / 10;
    SDL_Rect player_rect = {stage_rect.x + stage_padding, stage_rect.y + stage_padding, stage_rect.w - 2 * stage_padding, stage_rect.h - 2 * stage_padding};
    if (!draw_image_in_rectangle(game_window->renderer, player_rect, "../assets/player/image/player.png",
                                 player_orientation, false, ALIGN_START, ALIGN_START)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int print_player_on_stage_cli(game_window_t * game_window, orientation_t player_orientation, rect_t stage_rect) {
    int stage_padding = (int)stage_rect.h / 5;
    rect_t player_rect = {stage_rect.x + stage_padding, stage_rect.y + stage_padding, stage_rect.w - 2 * stage_padding, stage_rect.h - 2 * stage_padding};
    cli_draw_fill_rect(game_window->matrix, player_rect, (cli_char_t){'O', BLACK});

    return EXIT_SUCCESS;
}
