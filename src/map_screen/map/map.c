#include "map.h"
#include "../../sdl_utils/sdl_utils.h"
#include "../stage/display/display.h"

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

int get_map_dimensions(map_t *map, int * width, int * height, int * initial_x, int * initial_y) {
    if (!map || !width || !height || !initial_x || !initial_y) {
        fprintf(stderr, "\nget_map_dimensions error: please provide all necessary arguments.");
        return EXIT_FAILURE;
    }

    int max_x = 0;
    int max_y = 0;
    int min_x = 0;
    int min_y = 0;

    get_stage_dimensions(map->first_stage, 0, 0, &max_x, &max_y, &min_x, &min_y);

    *width = (max_x - min_x) + 1;
    *height = (max_y - min_y) + 1;

    *initial_x = 0 - min_x;
    *initial_y = 0 - min_y;

    return EXIT_SUCCESS;
}
