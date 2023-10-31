#include "map.h"
#include "../../ui_utils/sdl_utils/sdl_utils.h"
#include "../stage/display/display.h"

map_t * json_to_map(json_t * json_map) {
    if (!json_map) {
        fprintf(stderr, "\njson_to_map error: empty json_map");
        return NULL;
    }
    map_t * result = malloc(sizeof(map_t));

    if (json_map->nb_elements < 2) {
        fprintf(stderr, "\njson_to_map error: too few elements in json");
        return NULL;
    }

    json_t * name = get_object_at_key(json_map, "name");
    json_t * stages = get_object_at_key(json_map, "stages");
    if (!name || !stages || name->type != 's' || stages->type != 'a' || stages->nb_elements < 1) {
        fprintf(stderr, "\njson_to_map error: wrong map name or stages");
        return NULL;
    }

    result->name = name->string;

    stage_t * first_stage = json_to_stage(&(stages->values[0]), true);
    if (!first_stage) {
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

map_t * get_map_from_file(char * file_path) {
    json_t * json_map = get_json_from_file(file_path);
    if (!json_map) {
        fprintf(stderr, "\nget_map_from_file error: could not retrieve map from file.");
        return NULL;
    }
    map_t * map = json_to_map(json_map);
    if (!map) {
        fprintf(stderr, "\nget_map_from_file error: could not convert json to map.");
        return NULL;
    }

    return map;
}

void free_map(map_t * map) {
    if(map) {
        if(map->name) {
            free(map->name);
        }
        free_stages(map->first_stage);
        free(map);
    }
}

json_t * map_to_json(map_t * map) {
    if (!map) {
        fprintf(stderr, "map_to_json error: invalid map\n");
        return NULL;
    }

    json_t * json_map = malloc(sizeof(json_t));
    if (!json_map) {
        fprintf(stderr, "map_to_json error: memory allocation failed\n");
        return NULL;
    }

    json_map->type = 'o';
    json_map->nb_elements = 0;
    json_map->keys = NULL;
    json_map->values = NULL;

    // name
    if (map->name) {
        json_t * json_name = malloc(sizeof(json_t));
        json_name->type = 's';
        json_name->string = malloc(strlen(map->name) + 1);
        strcpy(json_name->string, map->name);
        add_key_value_to_object(json_map, "name", json_name);
    }

    // array containing stages
    json_t * stages_array = malloc(sizeof(json_t));
    stages_array->type = 'a';
    stages_array->nb_elements = 1;
    stages_array->values = malloc(stages_array->nb_elements * sizeof(json_t));

    // stages
    if (map->first_stage) {
        json_t * json_first_stage = stage_to_json(map->first_stage);
        if (json_first_stage) {
            stages_array->values[0] = *json_first_stage;
        }
    }

    add_key_value_to_object(json_map, "stages", stages_array);

    return json_map;
}
