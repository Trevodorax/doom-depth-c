#include "fight.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

fight_t * json_to_fight(Json * fight_json) {
    if (!fight_json || fight_json->type != 'o') {
        fprintf(stderr, "\njson_to_fight error: invalid input json\n");
        return NULL;
    }

    fight_t *result = malloc(sizeof(fight_t));
    if (!result) {
        fprintf(stderr, "\njson_to_fight error: memory allocation failed\n");
        return NULL;
    }

    Json *min_nb_enemies = get_object_at_key(fight_json, "min_nb_enemies");
    Json *max_nb_enemies = get_object_at_key(fight_json, "max_nb_enemies");
    if (!min_nb_enemies || min_nb_enemies->type != 'n' || min_nb_enemies->number == 0 || !max_nb_enemies || max_nb_enemies->type != 'n' || max_nb_enemies->number == 0) {
        free(result);
        return NULL;
    }
    result->min_nb_enemies = min_nb_enemies->number;
    result->max_nb_enemies = max_nb_enemies->number;

    Json *enemies_list = get_object_at_key(fight_json, "enemies_list");
    if (!enemies_list || enemies_list->type != 'a') {
        fprintf(stderr, "\njson_to_fight error: enemies_list not found or invalid\n");
        free(result);
        return NULL;
    }

    result->enemies_list = malloc(enemies_list->nb_elements * sizeof(char *));
    for (unsigned int i = 0; i < enemies_list->nb_elements; i++) {
        if(enemies_list->values[i].type != 's') {
            fprintf(stderr, "\njson_to_fight error: enemy name is not a string.\n");
            return NULL;
        }
        result->enemies_list[i] = strdup(enemies_list->values[i].string);
    }

    Json *chances = get_object_at_key(fight_json, "enemies_chances_to_appear");
    if(!chances || chances->type != 'a') {
        fprintf(stderr, "\njson_to_fight error: enemies_chances_to_appear not found or invalid\n");
        free(result->enemies_list);
        free(result);
        return NULL;
    }

    result->enemies_chances_to_appear = malloc(enemies_list->nb_elements * sizeof(float));
    for (unsigned int i = 0; i < chances->nb_elements; i++) {
        if(chances->values[i].type != 'n') {
            fprintf(stderr, "\njson_to_fight error: enemy chances to appear is not a number.");
            return NULL;
        }
        result->enemies_chances_to_appear[i] = ((float)chances->values[i].number) / 100;
    }

    return result;
}