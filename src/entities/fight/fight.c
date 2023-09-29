#include "fight.h"
#include <stdio.h>
#include <stdlib.h>

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

    // Set values from JSON
    Json *nb_enemies = get_object_at_key(fight_json, "nb_monsters");
    if (!nb_enemies || nb_enemies->type != 'n' || nb_enemies->number == 0) {
        free(result);
        return NULL;
    }
    result->nb_enemies = nb_enemies->number;

    Json *chances = get_object_at_key(fight_json, "enemy_chances_to_appear");
    if(!chances || chances->type != 'a') {
        fprintf(stderr, "\njson_to_fight error: enemy_chances_to_appear not found or invalid\n");
        free(result);
        return NULL;
    }

    result->enemy_chances_to_appear = (float *)malloc(result->nb_enemies * sizeof(float));
    for (unsigned int i = 0; i < result->nb_enemies; i++) {
        if(chances->values[i].type != 'n') {
            fprintf(stderr, "\njson_to_fight error: enemy chances to appear is not a number.");
            return NULL;
        }
        result->enemy_chances_to_appear[i] = ((float)chances->values[i].number) / 100;
    }

    return result;
}