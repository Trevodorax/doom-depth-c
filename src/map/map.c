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
 * @warning The stages must be uncounted before calling
 * @param game_window The window to print the stages on
 * @param stages The stage to be displayed
 * @param x_coord The x-coord of the stage
 * @param y_coord The y-coord of the stage
 * @param stage_size The size of the stage
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int print_stages_rec(game_window_t * game_window, stage_t * stages, int x_coord, int y_coord, int stage_size, bool with_players);

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
int print_stages(game_window_t * game_window, stage_t * stages, int x_coord, int y_coord, int stage_size, bool with_players);


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
int print_stage(game_window_t * game_window, stage_t * stage, int x_coord, int y_coord, int stage_size, bool with_players);

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
int print_player_on_stage(game_window_t * game_window, orientation_t player_orientation, SDL_Rect stage_rect);

/**
 * @brief Moves the player and returns the stage he is on
 *
 * @param player_stage The stage the player is currently on
 * @param direction In which direction the player desires to move
 * @sideeffects Modifies the graph to make everything start from the player position
 * @return The stage the player is on after the move
 */
stage_t * move_player(stage_t * player_stage, orientation_t direction);

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

    stage_t * player_stage = get_player_stage(map->first_stage);

    printf("\nMap name: %s\n", map->name);

    SDL_Event e;
    bool quit = false;
    while (!quit){
        SDL_Delay(50);
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_z:
                        player_stage = move_player(player_stage, NORTH);
                        break;
                    case SDLK_d:
                        player_stage = move_player(player_stage, EAST);
                        break;
                    case SDLK_s:
                        player_stage = move_player(player_stage, SOUTH);
                        break;
                    case SDLK_q:
                        player_stage = move_player(player_stage, WEST);
                        break;
                    default:
                        // Handle other keys
                        break;
                }
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
        map->first_stage = player_stage;
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

    print_stages(game_window, map->first_stage, initial_x, initial_y, stage_size, true);

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

    get_stage_dimensions(map->first_stage, 0, 0, &max_x, &max_y, &min_x, &min_y);

    *width = (max_x - min_x) + 1;
    *height = (max_y - min_y) + 1;

    *initial_x = 0 - min_x;
    *initial_y = 0 - min_y;

    return EXIT_SUCCESS;
}

int print_stages(game_window_t * game_window, stage_t * stages, int x_coord, int y_coord, int stage_size, bool with_players) {
    uncount_stages(stages);
    print_stages_rec(game_window, stages, x_coord, y_coord, stage_size, with_players);

    return EXIT_SUCCESS;
}

int print_stages_rec(game_window_t * game_window, stage_t * stages, int x_coord, int y_coord, int stage_size, bool with_players) {
    if(!stages || stages->counted) {
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

int print_stage(game_window_t * game_window, stage_t * stage, int x_coord, int y_coord, int stage_size, bool with_players) {
    SDL_Rect stage_rect =  {x_coord, y_coord, stage_size, stage_size};
    SDL_Color * stage_color = get_stage_color(stage);
    if(draw_fill_rect(stage_rect, *stage_color, game_window->renderer) == EXIT_FAILURE) {
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

    return next_stage;
}
