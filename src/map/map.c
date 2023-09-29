#include "map.h"
#include "../storage/json/json.h"
#include "../sdl_utils/sdl_utils.h"

/**
 * @brief displays map on the window
 *
 * @param game_window The window to print the map on
 * @param map The map to print there
 * @sideeffects Displays the map on the window, removing any previously printed content
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int display_map(game_window_t * game_window, map_t * map);

/**
 * @brief Converts a json into a map (does all the required checks)
 *
 * @param json_map The json to convert into a map
 * @error If the json is not a correct map, will return NULL
 * @return The parsed map or NULL
 */
map_t * json_to_map(Json * json_map);

/**
 * @brief Gets the height and width of a map
 *
 * @param map The map for which we get the dimensions
 * @param width Where the width will be stored
 * @param height Where the height will be stored
 * @param initial_x Where the x-coord of the first stage will be stored
 * @param initial_y Where the y-coord of the first stage will be stored
 * @sideeffects Sets the width and height to the dimensions of the map
 * @error If the map is not correct, this function will die (the checks must have been done before)
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int get_map_dimensions(map_t *map, int * width, int * height, int * initial_x, int * initial_y);

/**
 * @brief Displays the given stage and its stages recursively
 *
 * @param game_window The window to print the stages on
 * @param stages The stage to be displayed
 * @param x_coord The x-coord of the stage
 * @param y_coord The y-coord of the stage
 * @param stage_size The size of the stage
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int print_stages(game_window_t * game_window, stage_t * stages, int x_coord, int y_coord, int stage_size);

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
int print_stage(game_window_t * game_window, stage_t * stage, int x_coord, int y_coord, int stage_size);

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

    // process all dimensions
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(game_window->window, &window_width, &window_height);

    stage_t * test = map->first_stage;

    int map_width;
    int map_height;
    int initial_x;
    int initial_y;
    get_map_dimensions(map, &map_width, &map_height, &initial_x, &initial_y);

    int map_size = map_width > map_height ? map_width : map_height;
    int window_size = window_width < window_height ? window_width : window_height;

    int stage_size = window_size / map_size;

    SDL_Color background_color = { 255, 255, 255, 255};
    if(set_background_color(game_window->renderer, background_color) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    uncount_stages(map->first_stage);
    print_stages(game_window, map->first_stage, initial_x, initial_y, stage_size);

    SDL_RenderPresent(game_window->renderer);

    return EXIT_SUCCESS;
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

    uncount_stages(map->first_stage);
    get_stage_dimensions(map->first_stage, 0, 0, &max_x, &max_y, &min_x, &min_y);

    *width = (max_x - min_x) + 1;
    *height = (max_y - min_y) + 1;

    *initial_x = 0 - min_x;
    *initial_y = 0 - min_y;

    return EXIT_SUCCESS;
}

int print_stages(game_window_t * game_window, stage_t * stages, int x_coord, int y_coord, int stage_size) {
    if(!stages || stages->counted) {
        return EXIT_SUCCESS;
    }

    stages->counted = true;

    print_stage(game_window, stages, x_coord * stage_size, y_coord * stage_size, stage_size);

    print_stages(game_window, stages->top, x_coord, y_coord - 1, stage_size);
    print_stages(game_window, stages->right, x_coord + 1, y_coord, stage_size);
    print_stages(game_window, stages->bottom, x_coord, y_coord + 1, stage_size);
    print_stages(game_window, stages->left, x_coord - 1, y_coord, stage_size);

    return EXIT_SUCCESS;
}

int print_stage(game_window_t * game_window, stage_t * stage, int x_coord, int y_coord, int stage_size) {
    SDL_Rect stage_rect =  {x_coord, y_coord, stage_size, stage_size};
    SDL_Color * stage_color = get_stage_color(stage);

    if(!draw_fill_rect(stage_rect, *stage_color, game_window->renderer)) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
