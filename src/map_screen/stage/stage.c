#include "stage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../ui_utils/sdl_utils/sdl_utils.h"
#include "../../logs/log.h"

// internal functions, must be used only by the functions exposed in the header file (or the stages must be uncounted before)
void get_stage_dimensions_rec(stage_t * stage, int x, int y, int * max_x, int * max_y, int * min_x, int * min_y);
stage_t * get_player_stage_rec(stage_t *stages);

stage_t * json_to_stage(json_t * json_stage, bool first_stage) {
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
    result->is_done = false;
    result->has_linked_map = false;
    result->fight = NULL;
    result->top = result->right = result->bottom = result->left = NULL;
    result->counted = false;

    if (first_stage) {
        result->player = malloc(sizeof(player_t));
        result->player_orientation = SOUTH;
    }

    json_t *done = get_object_at_key(json_stage, "done");
    if (done && done->type == 'n')
        result->is_done = done->number == 1;

    fight_t * fight = json_to_fight(json_stage);
    if (fight) {
        result->fight = fight;
    }

    // linked map stuff
    json_t *linked_map = get_object_at_key(json_stage, "linked_map");
    if (linked_map && linked_map->type == 's') {
        result->has_linked_map = true;
        result->linked_map_file_path = malloc(strlen(linked_map->string) + 1);
        strcpy(result->linked_map_file_path, linked_map->string);
    }

    // set stage type
    if (result->fight) {
        result->type = FIGHT;
    } else {
        result->type = EMPTY;
    }

    json_t * treasure = get_object_at_key(json_stage, "treasure");
    if (treasure && treasure->type == 'o') {
        result->type = TREASURE;
        // TODO: set treasure
    }

    json_t * shop = get_object_at_key(json_stage, "shop");
    if (shop && shop->type == 'o') {
        result->type = SHOP;
        // TODO: set shop
    }

    // recursive calls for next stages
    json_t *top = get_object_at_key(json_stage, "top");
    if (top) {
        result->top = json_to_stage(top, false);
    }

    json_t *right = get_object_at_key(json_stage, "right");
    if (right) {
        result->right = json_to_stage(right, false);
    }

    json_t *bottom = get_object_at_key(json_stage, "bottom");
    if (bottom) {
        result->bottom = json_to_stage(bottom, false);
    }

    json_t *left = get_object_at_key(json_stage, "left");
    if (left) {
        result->left = json_to_stage(left, false);
    }

    return result;
}

void uncount_stages(stage_t * stage) {
    if (!stage) {
        return;
    }

    stage->counted = false;

    uncount_stages(stage->top);
    uncount_stages(stage->right);
    uncount_stages(stage->bottom);
    uncount_stages(stage->left);
}

void get_stage_dimensions_rec(stage_t *stage, int x, int y, int * max_x, int * max_y, int * min_x, int * min_y) {
    if (!stage || stage->counted) {
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
    if (!stage) {
        return WHITE;
    }

    switch (stage->type) {
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

void free_stage(stage_t * stage) {
    if (stage == NULL || stage->counted) {
        return;
    }

    if (stage->linked_map_file_path) {
        free(stage->linked_map_file_path);
    }
    if (stage->fight) {
        free_fight(stage->fight);
    }
    if (stage->player) {
        // TODO: free the player
    }

    free(stage);
}

void free_stages(stage_t * stages) {
    if (stages == NULL) {
        return;
    }

    stages->counted = true;

    free_stage(stages->top);
    free_stage(stages->right);
    free_stage(stages->bottom);
    free_stage(stages->left);
}

json_t * stage_to_json_rec(stage_t * stage);
json_t * stages_to_json(stage_t * stage) {
    uncount_stages(stage);
    return stage_to_json_rec(stage);
}

json_t * stage_to_json_rec(stage_t * stage) {
    if (!stage || stage->counted) {
        return NULL;
    }

    stage->counted = true;

    json_t * json_stage = malloc(sizeof(json_t));
    if (!json_stage) {
        global_logger->error("\nstage_to_json error: memory allocation failed");
        return NULL;
    }

    json_stage->type = 'o';
    json_stage->nb_elements = 0;
    json_stage->keys = NULL;
    json_stage->values = NULL;

    // done
    json_t * done = malloc(sizeof(json_t));
    done->type = 'n';
    done->number = stage->is_done ? 1 : 0;
    add_key_value_to_object(&json_stage, "done", done);

    // fight
    if (stage->type == FIGHT && stage->fight) {
        add_fight_to_json_object(json_stage, stage->fight);
    }

    // linked_map
    if (stage->has_linked_map && stage->linked_map_file_path) {
        json_t * linked_map = malloc(sizeof(json_t));
        linked_map->type = 's';
        linked_map->string = strdup(stage->linked_map_file_path);  // duplicating string
        add_key_value_to_object(&json_stage, "linked_map", linked_map);
    }

    // type
    if (stage->type == TREASURE) {
        // TODO: add treasure
    } else if (stage->type == SHOP) {
        // TODO: add shop
    }

    // calls for other stages
    if (stage->top) {
        json_t * json_top = stage_to_json_rec(stage->top);
        if (json_top) {
            add_key_value_to_object(&json_stage, "top", json_top);
        }
    }

    if (stage->right) {
        json_t * json_right = stage_to_json_rec(stage->right);
        if (json_right) {
            add_key_value_to_object(&json_stage, "right", json_right);
        }
    }

    if (stage->bottom) {
        json_t * json_bottom = stage_to_json_rec(stage->bottom);
        if (json_bottom) {
            add_key_value_to_object(&json_stage, "bottom", json_bottom);
        }
    }

    if (stage->left) {
        json_t * json_left = stage_to_json_rec(stage->left);
        if (json_left) {
            add_key_value_to_object(&json_stage, "left", json_left);
        }
    }

    return json_stage;
}
