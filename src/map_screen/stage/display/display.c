#include "display.h"

#include "../../../sdl_utils/sdl_utils.h"

int print_stages_rec(game_window_t *game_window, stage_t *stages, SDL_Texture **stage_textures, int x_coord, int y_coord, int stage_size, bool with_players);

SDL_Texture ** get_stage_textures(SDL_Renderer * renderer) {
    SDL_Texture ** stage_textures = malloc(sizeof(SDL_Texture*) * STAGE_TYPE_COUNT);
    for(int i = 0; i < STAGE_TYPE_COUNT; i++) {
        stage_textures[i] = get_image_texture(renderer, stage_texture_files[i]);
        if(!stage_textures[i]) {
            // cleanup and leave
            fprintf(stderr, "\nget_stage_textures error: failed to get the file textures.");
            for(int j = 0; j < i; j++) {
                free(stage_textures[j]);
            }
            free(stage_textures);
            return NULL;
        }
    }
    return stage_textures;
}

int print_stages(game_window_t *game_window, stage_t *stages, SDL_Texture **stage_textures, int x_coord, int y_coord,
                 int stage_size, bool with_players) {
    uncount_stages(stages);
    print_stages_rec(game_window, stages, stage_textures, x_coord, y_coord, stage_size, with_players);

    return EXIT_SUCCESS;
}

int print_stages_rec(game_window_t *game_window, stage_t *stages, SDL_Texture **stage_textures, int x_coord, int y_coord,
                     int stage_size, bool with_players) {
    if(!stages || stages->counted) {
        return EXIT_SUCCESS;
    }

    stages->counted = true;

    print_stage(game_window, stages, stage_textures, x_coord * stage_size, y_coord * stage_size, stage_size, with_players);

    print_stages_rec(game_window, stages->top, stage_textures, x_coord, y_coord - 1, stage_size, with_players);
    print_stages_rec(game_window, stages->right, stage_textures, x_coord + 1, y_coord, stage_size, with_players);
    print_stages_rec(game_window, stages->bottom, stage_textures, x_coord, y_coord + 1, stage_size, with_players);
    print_stages_rec(game_window, stages->left, stage_textures, x_coord - 1, y_coord, stage_size, with_players);

    return EXIT_SUCCESS;
}

int print_stage(game_window_t *game_window, stage_t *stage, SDL_Texture **stage_textures, int x_coord, int y_coord,
                int stage_size, bool with_players) {
    SDL_Rect stage_rect =  {x_coord, y_coord, stage_size, stage_size};
    SDL_Color * stage_color = get_stage_color(stage);

    if(SDL_RenderCopy(game_window->renderer, stage_textures[stage->type], NULL, &stage_rect) != 0) {
        return EXIT_FAILURE;
    }

    if(stage->player && with_players) {
        print_player_on_stage(game_window, stage->player_orientation, stage_rect);
    }

    return EXIT_SUCCESS;
}

int print_player_on_stage(game_window_t * game_window, orientation_t player_orientation, SDL_Rect stage_rect) {
    int stage_padding = stage_rect.h / 10;
    SDL_Rect player_rect = {stage_rect.x + stage_padding, stage_rect.y + stage_padding, stage_rect.w - 2 * stage_padding, stage_rect.h - 2 * stage_padding};
    if (!draw_image_in_rectangle(game_window->renderer, player_rect, "../assets/player.png", player_orientation)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
