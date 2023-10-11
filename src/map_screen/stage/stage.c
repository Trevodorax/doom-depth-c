#include "stage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../sdl_utils/sdl_utils.h"

// internal functions, must be used only by the functions exposed in the header file (or the stages must be uncounted before)
void get_stage_dimensions_rec(stage_t *stage, int x, int y, int * max_x, int * max_y, int * min_x, int * min_y);
stage_t *get_player_stage_rec(stage_t *stages);

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

    if(result->is_start) {
        result->player = malloc(sizeof(player_t));
        result->player_orientation = SOUTH;
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

    Json * shop = get_object_at_key(json_stage, "shop");
    if(shop && shop->type == 'o') {
        result->type = SHOP;
        // TODO: set shop
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

void get_stage_dimensions_rec(stage_t *stage, int x, int y, int * max_x, int * max_y, int * min_x, int * min_y) {
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

    get_stage_dimensions_rec(stage->top, x, y - 1, max_x, max_y, min_x, min_y);
    get_stage_dimensions_rec(stage->right, x + 1, y, max_x, max_y, min_x, min_y);
    get_stage_dimensions_rec(stage->bottom, x, y + 1, max_x, max_y, min_x, min_y);
    get_stage_dimensions_rec(stage->left, x - 1, y, max_x, max_y, min_x, min_y);
}

void get_stage_dimensions(stage_t *stage, int x, int y, int * max_x, int * max_y, int * min_x, int * min_y) {
    uncount_stages(stage);
    get_stage_dimensions_rec(stage, x, y, max_x, max_y, min_x, min_y);
}

color_code_t get_stage_color(stage_t * stage) {
    if(!stage) {
        return WHITE;
    }

    switch(stage->type) {
        case EMPTY:
            return GREEN;
        case FIGHT:
            return RED;
        case SHOP:
            return BLUE;
        case TREASURE:
            return YELLOW;
        default:
            return WHITE;
    }
}

stage_t *get_player_stage(stage_t *stages) {
    uncount_stages(stages);
    return get_player_stage_rec(stages);
}

stage_t *get_player_stage_rec(stage_t *stages) {
    if (!stages || stages->counted) {
        return NULL;
    }

    stages->counted = true;

    if (stages->player != NULL) {
        return stages;
    }

    stage_t *result;

    result = get_player_stage_rec(stages->top);
    if (result != NULL) return result;

    result = get_player_stage_rec(stages->right);
    if (result != NULL) return result;

    result = get_player_stage_rec(stages->bottom);
    if (result != NULL) return result;

    result = get_player_stage_rec(stages->left);
    if (result != NULL) return result;

    return NULL;
}
