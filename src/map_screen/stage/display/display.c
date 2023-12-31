#include "display.h"

#include "../../../ui_utils/sdl_utils/sdl_utils.h"
#include "../../../ui_utils/cli_utils/ascii_art/ascii_art.h"

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

ascii_art_t ** get_stage_ascii_arts(color_code_t color) {
    static ascii_art_t ** stage_ascii_arts = NULL;
    if (stage_ascii_arts) {
        return stage_ascii_arts;
    }

    stage_ascii_arts = calloc(STAGE_TYPE_COUNT, sizeof(ascii_art_t*));
    for (int i = 0; i < STAGE_TYPE_COUNT; i++) {
        stage_ascii_arts[i] = parse_ascii_art_file(stage_ascii_arts_files[i], color);
        if (!stage_ascii_arts[i]) {
            // cleanup and leave
            fprintf(stderr, "\nget_stage_ascii_arts error: failed to get the file ascii arts.");
            for (int j = 0; j < i; j++) {
                free(stage_ascii_arts[j]);
            }
            free(stage_ascii_arts);
            return NULL;
        }
    }

    return stage_ascii_arts;
}

int print_stages(game_window_t * game_window, stage_t * stages, int x_coord, int y_coord, int stage_size, bool with_players) {
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

    } else if(stage->type == TREASURE && stage->is_done) {
        stage_texture = stage_textures[TREASURE_DONE];

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

    ascii_art_t ** stage_ascii_arts = get_stage_ascii_arts(game_window->cli_color_palette->text);
    if (stage_ascii_arts) {
        // retrieve right ascii art for stage
        ascii_art_t * stage_ascii_art = NULL;
        if(stage->type == FIGHT && stage->is_done) {
            stage_ascii_art = stage_ascii_arts[FIGHT_DONE];

        } else if(stage->type == TREASURE && stage->is_done) {
            stage_ascii_art = stage_ascii_arts[TREASURE_DONE];

        } else {
            stage_ascii_art = stage_ascii_arts[stage->type];
        }

        // print it in the stage
        cli_matrix_t * ascii_art_matrix = pick_ascii_art_version(stage_ascii_art, stage_size - 1, stage_size - 1);
        if (!ascii_art_matrix) {
            return EXIT_FAILURE;
        }
        rect_t ascii_art_rect = {
                stage_rect.x + 1,
                stage_rect.y + 1,
                stage_rect.w - 2,
                stage_rect.h - 2
        };
        if (cli_copy_matrix(game_window->matrix, ascii_art_rect, ascii_art_matrix, ALIGN_CENTER, ALIGN_CENTER) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }

    cli_draw_stroke_rect(game_window->matrix, stage_rect, (cli_char_t){'#', stage_color});

    if (stage->player && with_players) {
        print_player_on_stage_cli(game_window, stage->player_orientation, stage_rect);
    }

    return EXIT_SUCCESS;
}

int print_player_on_stage_gui(game_window_t * game_window, orientation_t player_orientation, SDL_Rect stage_rect) {
    int stage_padding = stage_rect.h / 10;
    SDL_Rect player_rect = {stage_rect.x + stage_padding, stage_rect.y + stage_padding, stage_rect.w - 2 * stage_padding, stage_rect.h - 2 * stage_padding};
    if (draw_image_in_rectangle(game_window->renderer, player_rect, "../assets/player/image/player.png",
                                 player_orientation, false, ALIGN_START, ALIGN_START) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int print_player_on_stage_cli(game_window_t * game_window, orientation_t player_orientation, rect_t stage_rect) {
    int stage_padding = (int)stage_rect.h / 5;
    if (stage_padding < 1) {
        stage_padding = 1;
    }

    rect_t player_rect = {stage_rect.x + stage_padding, stage_rect.y + stage_padding, stage_rect.w - 2 * stage_padding, stage_rect.h - 2 * stage_padding};
    print_ascii_art_in_rectangle(game_window->matrix, "../assets/player/ascii/player.asciiart", player_rect, ALIGN_CENTER, ALIGN_CENTER, game_window->cli_color_palette->highlight);

    return EXIT_SUCCESS;
}
