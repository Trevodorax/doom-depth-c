#include "map.h"
#include "../storage/json/json.h"
#include "../sdl_utils/sdl_utils.h"
#include "stage/stage.h"

int display_map(game_window_t * game_window, map_t * map);
map_t * json_to_map(Json * json_map);

int map_screen(game_window_t *game_window, char *map_file) {
    Json * json_map = get_json_from_file(map_file);
    if(!json_map) {
        fprintf(stderr, "\nmap_screen error: could not retrieve map from file.");
        return EXIT_FAILURE;
    }

    map_t * map = json_to_map(json_map);
    if(!map) {
        fprintf(stderr, "\nmap_screen error: could not convert json to map.");
        return EXIT_FAILURE;
    }

    printf("\nMap name: %s\n", map->name);

    SDL_Event e;
    int quit = 0;
    while (!quit){
        SDL_Delay(50);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = 1;
            }
            if (e.type == SDL_KEYDOWN){
                quit = 1;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = 1;
            }
        }
        display_map(game_window, map);
    }

    return EXIT_SUCCESS;
}

map_t * json_to_map(Json * json_map) {
    if(!json_map) {
        fprintf(stderr, "\njson_to_map error: empty json_map");
        return NULL;
    }
    map_t * result = malloc(sizeof(map_t));

    if(json_map->nb_elements < 2) {
        fprintf(stderr, "\njson_to_map error: too few elements in json");
        return NULL;
    }

    Json * name = get_object_at_key(json_map, "name");
    Json * stages = get_object_at_key(json_map, "stages");
    if(!name || !stages || name->type != 's' || stages->type != 'a' || stages->nb_elements < 1) {
        fprintf(stderr, "\njson_to_map error: wrong map name or stages");
        return NULL;
    }

    result->name = name->string;

    stage_t * first_stage = json_to_stage(&(stages->values[0]));
    if(!first_stage) {
        fprintf(stderr, "\njson_to_map error: error retrieving stages.");
        return NULL;
    }

    result->first_stage = first_stage;

    return result;
}

int display_map(game_window_t * game_window, map_t * map) {
    if(!game_window || !map) {
        fprintf(stderr, "\ndisplay_map error: please provide all necessary arguments");
        return EXIT_FAILURE;
    }

    SDL_Color background_color = { 255, 255, 255, 255};
    if(set_background_color(game_window->renderer, background_color) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    SDL_Rect stage_rect =  {0, 0, 50, 50};
    SDL_Color stage_color = { 150, 0, 0, 255};
    if(draw_fill_rect(stage_rect, stage_color, game_window->renderer) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
}
