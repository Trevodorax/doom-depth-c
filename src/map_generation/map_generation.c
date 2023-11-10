#include "map_generation.h"

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
