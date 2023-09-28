#include "stage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

stage_t * json_to_stage(Json * json_stage) {
    if (!json_stage || json_stage->type != 'o') {
        fprintf(stderr, "json_to_stage error: invalid input json\n");
        return NULL;
    }

    stage_t *result = malloc(sizeof(stage_t));
    if (!result) {
        fprintf(stderr, "json_to_stage error: memory allocation failed\n");
        return NULL;
    }

    // default values
    result->is_start = false;
    result->is_done = false;
    result->has_linked_map = false;
    result->fight = NULL;
    result->top = result->right = result->bottom = result->left = NULL;

    Json * start = get_object_at_key(json_stage, "start");
    if (start && start->type == 'n') {
        result->is_start = start->number;
    }

    Json *done = get_object_at_key(json_stage, "done");
    if (done && done->type == 'n')
        result->is_done = done->number;

    fight_t * fight = json_to_fight(json_stage);
    if(fight) {
        result->fight = fight;
    }

    // linked map stuff
    Json *linked_map = get_object_at_key(json_stage, "linked_map");
    if (linked_map && linked_map->type == 's') {
        result->has_linked_map = true;
        strcpy(result->linked_map_file_path, linked_map->string);
    }

    // recursive calls for next stages
    Json *top = get_object_at_key(json_stage, "top");
    if (top) {
        result->top = json_to_stage(top);
    }

    Json *right = get_object_at_key(json_stage, "right");
    if (right) {
        result->right = json_to_stage(right);
    }

    Json *bottom = get_object_at_key(json_stage, "bottom");
    if (bottom) {
        result->bottom = json_to_stage(bottom);
    }

    Json *left = get_object_at_key(json_stage, "left");
    if (left) {
        result->left = json_to_stage(left);
    }

    return result;
}


