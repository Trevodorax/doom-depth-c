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
    result->counted = false;

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
        result->linked_map_file_path = malloc(strlen(linked_map->string) + 1);
        strcpy(result->linked_map_file_path, linked_map->string);
    }

    // set stage type
    if(result->fight) {
        result->type = FIGHT;
    } else {
        result->type = EMPTY;
    }

    Json * treasure = get_object_at_key(json_stage, "treasure");
    if(treasure && treasure->type == 'o') {
        result->type = TREASURE;
        // TODO: set treasure
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

void uncount_stages(stage_t * stage) {
    if(!stage) {
        return;
    }

    stage->counted = false;

    uncount_stages(stage->top);
    uncount_stages(stage->right);
    uncount_stages(stage->bottom);
    uncount_stages(stage->left);
}

void get_stage_dimensions(stage_t *stage, int x, int y, int * max_x, int * max_y, int * min_x, int * min_y) {
    if(!stage || stage->counted) {
        return;
    }

    stage->counted = true;

    if (x > *max_x) {
        *max_x = x;
    }

    if (y > *max_y) {
        *max_y = y;
    }

    if (x < *min_x) {
        *min_x = x;
    }

    if (y < *min_y) {
        *min_y = y;
    }

    get_stage_dimensions(stage->top, x, y - 1, max_x, max_y, min_x, min_y);
    get_stage_dimensions(stage->right, x + 1, y, max_x, max_y, min_x, min_y);
    get_stage_dimensions(stage->bottom, x, y + 1, max_x, max_y, min_x, min_y);
    get_stage_dimensions(stage->left, x - 1, y, max_x, max_y, min_x, min_y);
}

SDL_Color * get_stage_color(stage_t * stage) {
    if(!stage) {
        fprintf(stderr, "\nget_stage_color error: please provide all the necessary arguments");
        return NULL;
    }

    SDL_Color  * stage_color = malloc(sizeof(SDL_Color));

    switch(stage->type) {
        case EMPTY:
            stage_color->r = 0;
            stage_color->g = 150;
            stage_color->b = 0;
            stage_color->a = 255;
            break;
        case FIGHT:
            stage_color->r = 150;
            stage_color->g = 0;
            stage_color->b = 0;
            stage_color->a = 255;
            break;
        case SHOP:
            stage_color->r = 0;
            stage_color->g = 0;
            stage_color->b = 150;
            stage_color->a = 255;
            break;
        case TREASURE:
            stage_color->r = 150;
            stage_color->g = 150;
            stage_color->b = 0;
            stage_color->a = 255;
            break;
        default:
            stage_color->r = 0;
            stage_color->g = 0;
            stage_color->b = 0;
            stage_color->a = 255;
            break;
    }

    return stage_color;
}
