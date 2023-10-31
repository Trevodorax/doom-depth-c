#include "fight.h"
#include "../../logs/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

fight_t * json_to_fight(json_t * fight_json) {
    if (!fight_json || fight_json->type != 'o') {
        fprintf(stderr, "\njson_to_fight error: invalid input json\n");
        return NULL;
    }

    fight_t *result = malloc(sizeof(fight_t));
    if (!result) {
        fprintf(stderr, "\njson_to_fight error: memory allocation failed\n");
        return NULL;
    }

    json_t *min_nb_enemies = get_object_at_key(fight_json, "min_nb_enemies");
    json_t *max_nb_enemies = get_object_at_key(fight_json, "max_nb_enemies");
    if (!min_nb_enemies || min_nb_enemies->type != 'n' || min_nb_enemies->number == 0 || !max_nb_enemies || max_nb_enemies->type != 'n' || max_nb_enemies->number == 0) {
        free(result);
        return NULL;
    }
    result->min_nb_enemies = min_nb_enemies->number;
    result->max_nb_enemies = max_nb_enemies->number;

    json_t *enemies_list = get_object_at_key(fight_json, "enemies_list");
    if (!enemies_list || enemies_list->type != 'a') {
        fprintf(stderr, "\njson_to_fight error: enemies_list not found or invalid\n");
        free(result);
        return NULL;
    }
    result->enemies_size = enemies_list->nb_elements;

    result->enemies_list = malloc(enemies_list->nb_elements * sizeof(char *));
    for (unsigned int i = 0; i < enemies_list->nb_elements; i++) {
        if (enemies_list->values[i].type != 's') {
            fprintf(stderr, "\njson_to_fight error: enemy name is not a string.\n");
            return NULL;
        }
        result->enemies_list[i] = strdup(enemies_list->values[i].string);
    }


    json_t *chances = get_object_at_key(fight_json, "enemies_chances_to_appear");
    if (!chances || chances->type != 'a') {
        fprintf(stderr, "\njson_to_fight error: enemies_chances_to_appear not found or invalid\n");
        free(result->enemies_list);
        free(result);
        return NULL;
    }

    result->enemies_chances_to_appear = malloc(enemies_list->nb_elements * sizeof(float));
    for (unsigned int i = 0; i < chances->nb_elements; i++) {
        if (chances->values[i].type != 'n') {
            fprintf(stderr, "\njson_to_fight error: enemy chances to appear is not a number.");
            return NULL;
        }
        result->enemies_chances_to_appear[i] = chances->values[i].number;
    }

    return result;
}

void free_fight(fight_t * fight) {
    for(int i = 0; i < fight->enemies_size; i++) {
        free(fight->enemies_list[i]);
    }
    free(fight->enemies_list);

    free(fight);
}

void add_fight_to_json_object(json_t * object, fight_t * fight) {
    if (!fight || !object || object->type != 'o') {
        global_logger->error("\nfight_to_json error: wrong parameters");
        return;
    }

    // min_nb_enemies
    json_t * json_min_nb_enemies = malloc(sizeof(json_t));
    json_min_nb_enemies->type = 'n';
    json_min_nb_enemies->number = (int)fight->min_nb_enemies;
    add_key_value_to_object(&object, "min_nb_enemies", json_min_nb_enemies);

    // max_nb_enemies
    json_t * json_max_nb_enemies = malloc(sizeof(json_t));
    json_max_nb_enemies->type = 'n';
    json_max_nb_enemies->number = (int)fight->max_nb_enemies;
    add_key_value_to_object(&object, "max_nb_enemies", json_max_nb_enemies);

    // enemies_list
    if (fight->enemies_list) {
        json_t * json_enemies_list = malloc(sizeof(json_t));
        json_enemies_list->type = 'a';
        json_enemies_list->nb_elements = fight->enemies_size;
        json_enemies_list->values = malloc(fight->enemies_size * sizeof(json_t));

        for (int i = 0; i < fight->enemies_size; i++) {
            json_t * json_enemy_name = malloc(sizeof(json_t));
            json_enemy_name->type = 's';
            json_enemy_name->string = malloc(strlen(fight->enemies_list[i]) + 1);
            strcpy(json_enemy_name->string, fight->enemies_list[i]);
            json_enemies_list->values[i] = *json_enemy_name;
        }
        add_key_value_to_object(&object, "enemies_list", json_enemies_list);
    }

    // enemies_chances_to_appear
    if (fight->enemies_chances_to_appear) {
        json_t * json_chances = malloc(sizeof(json_t));
        json_chances->type = 'a';
        json_chances->nb_elements = fight->enemies_size;
        json_chances->values = malloc(fight->enemies_size * sizeof(json_t));

        for (int i = 0; i < fight->enemies_size; i++) {
            json_t * json_chance = malloc(sizeof(json_t));
            json_chance->type = 'n';
            json_chance->number = fight->enemies_chances_to_appear[i];
            json_chances->values[i] = *json_chance;
        }
        add_key_value_to_object(&object, "enemies_chances_to_appear", json_chances);
    }
}
