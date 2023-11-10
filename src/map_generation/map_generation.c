#include "map_generation.h"

int get_nb_neighbors(bool anti_collision_map[MAX_MAP_SIZE][MAX_MAP_SIZE], int x_coord, int y_coord);

fight_t * generate_harder_fight(fight_t * previous_fight) {
    srand(time(NULL));

    // init generated fight
    fight_t * generated_fight = calloc(1, sizeof(fight_t));
    generated_fight->min_nb_enemies = previous_fight->min_nb_enemies;
    generated_fight->max_nb_enemies = previous_fight->max_nb_enemies;
    generated_fight->enemies_size = previous_fight->enemies_size;
    generated_fight->enemies_list = calloc(previous_fight->enemies_size + 1, sizeof(char *));
    generated_fight->enemies_chances_to_appear = calloc(previous_fight->enemies_size + 1, sizeof(int));

    // parse the enemies list
    int * enemies_list_number = malloc((previous_fight->enemies_size + 1) * sizeof(int));
    for (int i = 0; i < previous_fight->enemies_size; i++) {
        enemies_list_number[i] = monster_name_to_number(previous_fight->enemies_list[i]);
    }

    // increase something
    bool did_increase = false;
    int increased_part = rand() % 2;
    if (increased_part == 0) {
        // increasing number of enemies
        if (generated_fight->min_nb_enemies < 7) {
            generated_fight->min_nb_enemies++;
            did_increase = true;
        }
        if (generated_fight->max_nb_enemies < 7) {
            generated_fight->max_nb_enemies++;
            did_increase = true;
        }
    }
    if (increased_part == 1 || !did_increase) {
        // check if possible to increase strength of an enemy
        bool possible = false;
        for (int i = 0; i < previous_fight->enemies_size; i++) {
            if (enemies_list_number[i] < NB_MONSTERS - 1) {
                possible = true;
                break;
            }
        }

        if (possible) {
            // choose enemy to improve
            int improved_enemy_index = 0;
            while (enemies_list_number[improved_enemy_index] == NB_MONSTERS - 1) {
                 improved_enemy_index = rand() % previous_fight->enemies_size;
            }

            // improve it
            enemies_list_number[improved_enemy_index]++;

            did_increase = true;
        }
    }

    // create the enemies list and chances to appear
    for (int i = 0; i < previous_fight->enemies_size; i++) {
        generated_fight->enemies_list[i] = strdup(number_to_monster_name(enemies_list_number[i]));
        generated_fight->enemies_chances_to_appear[i] = previous_fight->enemies_chances_to_appear[i];
    }

    // maybe add a possible enemy type
    bool should_add_enemy_type = rand() % 3 == 0;
    if (should_add_enemy_type) {
        generated_fight->enemies_size++;
        generated_fight->enemies_list[generated_fight->enemies_size - 1] = strdup(number_to_monster_name(enemies_list_number[0]));
        generated_fight->enemies_chances_to_appear[generated_fight->enemies_size - 1] = 100 / generated_fight->enemies_size;
    }

    return generated_fight;
}

int monster_name_to_number(char * monster_name) {
    for (int i = 0; i < NB_MONSTERS; i++) {
        if (strcmp(monster_name, monster_names[i]) == 0) {
            return i;
        }
    }

    return -1;
}

char * number_to_monster_name(int number) {
    if(number >= NB_MONSTERS) {
        return NULL;
    }

    return monster_names[number];
}

treasure_t * generate_treasure(fight_t * last_fight) {
    treasure_t * generated_treasure = calloc(1, sizeof(treasure_t));

    // default treasure
    if (!last_fight) {
        generated_treasure->coins = 20;
        return generated_treasure;
    }

    // process coins
    int nb_coins = 0;
    for (int i = 0; i < last_fight->enemies_size; i++) {
        nb_coins += (monster_name_to_number(last_fight->enemies_list[i]) + 1) * 5;
    }
    nb_coins *= (int)last_fight->min_nb_enemies;
    generated_treasure->coins = nb_coins;

    return generated_treasure;
}

fight_t * get_last_fight_rec(stage_t * stage);
fight_t * get_last_fight(stage_t * last_stage) {
    uncount_stages(last_stage);
    return get_last_fight_rec(last_stage);
}

fight_t * get_last_fight_rec(stage_t * stage) {
    if (stage->counted) {
        return NULL;
    }
    stage->counted = true;

    if (stage->type == FIGHT && stage->fight) {
        return stage->fight;
    }

    fight_t * top_fight = get_last_fight_rec(stage->top);
    fight_t * right_fight = get_last_fight_rec(stage->right);
    fight_t * bottom_fight = get_last_fight_rec(stage->bottom);
    fight_t * left_fight = get_last_fight_rec(stage->left);

    if (top_fight) {
        return top_fight;
    }
    if (right_fight) {
        return right_fight;
    }
    if (bottom_fight) {
        return bottom_fight;
    }
    if (left_fight) {
        return bottom_fight;
    }

    return NULL;
}

char * get_available_map_name(char * maps_folder_path) {
    int map_number = 1;
    FILE * file;
    char * full_path = calloc(256, sizeof(char));

    // get available number
    do {
        snprintf(full_path, 255, "%s/map_%d.json", maps_folder_path, map_number);

        file = fopen(full_path, "r");
        if (file) {
            fclose(file);
            map_number++;
        }
    } while (file);

    // get file name
    char * filename = calloc(20, sizeof(char));
    snprintf(filename, 19, "map_%d.json", map_number);

    // cleanup
    free(full_path);
    return filename;
}

stage_t * generate_stages_rec(fight_t * previous_fight, bool anti_collision_map[MAX_MAP_SIZE][MAX_MAP_SIZE], int x_coord, int y_coord) {
    srand(time(NULL));

    // out of bounds => no stage
    if (x_coord < 0 || y_coord < 0 || x_coord >= MAX_MAP_SIZE || y_coord >= MAX_MAP_SIZE) {
        return NULL;
    }
    // has neighbors (other than parent) => no stage
    int nb_neighbors = get_nb_neighbors(anti_collision_map, x_coord, y_coord);
    if(nb_neighbors > 1) {
        return NULL;
    }

    // init stage
    anti_collision_map[x_coord][y_coord] = true;
    stage_t * stage = calloc(1, sizeof(stage_t));
    stage->is_done = false;
    stage->counted = false;

    // get stage type
    int stage_type_random_number = rand() % 100;
    stage_type_t stage_type;
    if (stage_type_random_number < 20) {
        stage_type = EMPTY;
    } else if( stage_type_random_number < 30) {
        stage_type = SHOP;
    } else if( stage_type_random_number < 50) {
        stage_type = TREASURE;
    } else if( stage_type_random_number < 95) {
        stage_type = FIGHT;
    } else {
        stage_type = LINKED_MAP;
    }
    stage->type = stage_type;

    switch(stage_type) {
        case FIGHT: {
            fight_t * new_fight = generate_harder_fight(previous_fight);
            stage->fight = new_fight;
            previous_fight = new_fight; // to pass this new fight in the recursive calls
            break;
        }
        case TREASURE: {
            treasure_t * new_treasure = generate_treasure(previous_fight);
            stage->treasure = new_treasure;
        }
            break;
        case LINKED_MAP: {
            stage->has_linked_map = true;
            stage->linked_map_file_path = strdup("");
        }
            break;
        case SHOP:
            // add a shop if we decide to specify things in the shop in the future
        case EMPTY:
        default:
            break;
    }

    // stages that stop progression
    if (stage_type == LINKED_MAP || stage_type == TREASURE) {
        return stage;
    }

    // neighbors
    stage->top = generate_stages_rec(previous_fight, anti_collision_map, x_coord, y_coord - 1);
    stage->right = generate_stages_rec(previous_fight, anti_collision_map, x_coord + 1, y_coord);
    stage->bottom = generate_stages_rec(previous_fight, anti_collision_map, x_coord, y_coord + 1);
    stage->left = generate_stages_rec(previous_fight, anti_collision_map, x_coord - 1, y_coord);

    return stage;
}

int get_nb_neighbors(bool anti_collision_map[MAX_MAP_SIZE][MAX_MAP_SIZE], int x_coord, int y_coord) {
    int nb_neighbors = 0;

    if (x_coord < MAX_MAP_SIZE - 1 && anti_collision_map[x_coord + 1][y_coord]) {
        nb_neighbors++;
    }
    if (x_coord > 0 && anti_collision_map[x_coord - 1][y_coord]) {
        nb_neighbors++;
    }
    if (y_coord < MAX_MAP_SIZE - 1 && anti_collision_map[x_coord][y_coord + 1]) {
        nb_neighbors++;
    }
    if (y_coord > 0 && anti_collision_map[x_coord][y_coord - 1]) {
        nb_neighbors++;
    }

    return nb_neighbors;
}
