#include "monster.h"
#include "../../logs/log.h"

void *create_monster_from_db(sqlite3_stmt *stmt) {

    monster_t *monster = malloc(sizeof(monster_t));

    if (!monster) {
        global_logger->error("÷create_monster_from_db error: malloc failed\n");
        return NULL;
    }

    monster->id = sqlite3_column_int(stmt, 0);
    monster->type = (short)sqlite3_column_int(stmt, 1);
    monster->name = strdup((char *) sqlite3_column_text(stmt, 2));
    monster->hp = sqlite3_column_int(stmt, 3);
    monster->hp_max = sqlite3_column_int(stmt, 4);
    monster->attack = sqlite3_column_int(stmt, 5);
    monster->defense = sqlite3_column_int(stmt, 6);
    monster->image_path = strdup((char *) sqlite3_column_text(stmt, 7));
    monster->ascii_path = strdup((char *) sqlite3_column_text(stmt, 8));

    return monster;
}

monster_t * void_to_monster(void * void_monster) {
    monster_t * monster = (monster_t *)void_monster;

    if(!monster || !monster->name || !monster->image_path || !monster->ascii_path) {
        return NULL;
    }

    return monster;
}

monster_t* copy_monster(const monster_t* monster) {
    if (!monster) {
        return NULL;
    }

    monster_t* new_monster = (monster_t*) malloc(sizeof(monster_t));
    if (!new_monster) {
        return NULL;
    }

    // copy simple data types
    new_monster->id = monster->id;
    new_monster->type = monster->type;
    new_monster->hp = monster->hp;
    new_monster->hp_max = monster->hp_max;
    new_monster->attack = monster->attack;
    new_monster->defense = monster->defense;

    // deep copy pointers
    if (monster->name) {
        new_monster->name = strdup(monster->name);
        if (!new_monster->name) {
            free(new_monster); // Free allocated memory for new_monster
            return NULL; // memory allocation for name failed
        }
    } else {
        new_monster->name = NULL;
    }

    if (monster->image_path) {
        new_monster->image_path = strdup(monster->image_path);
        if (!new_monster->image_path) {
            free(new_monster->name); // Free allocated memory for name
            free(new_monster); // Free allocated memory for new_monster
            return NULL; // memory allocation for image_path failed
        }
    } else {
        new_monster->image_path = NULL;
    }

    if (monster->ascii_path) {
        new_monster->ascii_path = strdup(monster->ascii_path);
        if (!new_monster->ascii_path) {
            free(new_monster); // Free allocated memory for new_monster
            return NULL; // memory allocation for ascii_path failed
        }
    } else {
        new_monster->ascii_path = NULL;
    }

    return new_monster;
}

json_t * monster_to_json(monster_t *monster) {
    if (!monster) {
        global_logger->error("\nmonster_to_json error: wrong parameters");
        return NULL;
    }

    json_t * monster_json = malloc(sizeof(json_t));
    monster_json->type = 'o';

    // id
    json_t * json_id = malloc(sizeof(json_t));
    json_id->type = 'n';
    json_id->number = monster->id;
    add_key_value_to_object(&monster_json, "id", json_id);

    // type
    json_t * json_type = malloc(sizeof(json_t));
    json_type->type = 'n';
    json_type->number = monster->type;
    add_key_value_to_object(&monster_json, "type", json_type);

    // name
    if (monster->name) {
        json_t * json_name = malloc(sizeof(json_t));
        json_name->type = 's';
        json_name->string = strdup(monster->name);
        add_key_value_to_object(&monster_json, "name", json_name);
    }

    // hp
    json_t * json_hp = malloc(sizeof(json_t));
    json_hp->type = 'n';
    json_hp->number = (int)monster->hp;
    add_key_value_to_object(&monster_json, "hp", json_hp);

    // hp_max
    json_t * json_hp_max = malloc(sizeof(json_t));
    json_hp_max->type = 'n';
    json_hp_max->number = (int)monster->hp_max;
    add_key_value_to_object(&monster_json, "hp_max", json_hp_max);

    // attack
    json_t * json_attack = malloc(sizeof(json_t));
    json_attack->type = 'n';
    json_attack->number = (int)monster->attack;
    add_key_value_to_object(&monster_json, "attack", json_attack);

    // defense
    json_t * json_defense = malloc(sizeof(json_t));
    json_defense->type = 'n';
    json_defense->number = (int)monster->defense;
    add_key_value_to_object(&monster_json, "defense", json_defense);

    // image_path
    if (monster->image_path) {
        json_t * json_image_path = malloc(sizeof(json_t));
        json_image_path->type = 's';
        json_image_path->string = strdup(monster->image_path);
        add_key_value_to_object(&monster_json, "image_path", json_image_path);
    }

    // ascii_path
    if (monster->ascii_path) {
        json_t * json_ascii_path = malloc(sizeof(json_t));
        json_ascii_path->type = 's';
        json_ascii_path->string = strdup(monster->ascii_path);
        add_key_value_to_object(&monster_json, "ascii_path", json_ascii_path);
    }

    return monster_json;
}

monster_t * json_to_monster(json_t * object) {
    if (!object || object->type != 'o') {
        global_logger->error("\njson_to_monster error: wrong parameters");
        return NULL;
    }

    monster_t * monster = malloc(sizeof(monster_t));
    if (!monster) {
        global_logger->error("\njson_to_monster error: memory allocation failed");
        return NULL;
    }

    // Reset structure
    *monster = (monster_t){0};

    // id
    json_t * json_id = get_object_at_key(object, "id");
    if (json_id && json_id->type == 'n') {
        monster->id = json_id->number;
    }

    // type
    json_t * json_type = get_object_at_key(object, "type");
    if (json_type && json_type->type == 'n') {
        monster->type = json_type->number;
    }

    // name
    json_t * json_name = get_object_at_key(object, "name");
    if (json_name && json_name->type == 's') {
        monster->name = strdup(json_name->string);
    }

    // hp
    json_t * json_hp = get_object_at_key(object, "hp");
    if (json_hp && json_hp->type == 'n') {
        monster->hp = json_hp->number;
    }

    // hp_max
    json_t * json_hp_max = get_object_at_key(object, "hp_max");
    if (json_hp_max && json_hp_max->type == 'n') {
        monster->hp_max = json_hp_max->number;
    }

    // attack
    json_t * json_attack = get_object_at_key(object, "attack");
    if (json_attack && json_attack->type == 'n') {
        monster->attack = json_attack->number;
    }

    // defense
    json_t * json_defense = get_object_at_key(object, "defense");
    if (json_defense && json_defense->type == 'n') {
        monster->defense = json_defense->number;
    }

    // image_path
    json_t * json_image_path = get_object_at_key(object, "image_path");
    if (json_image_path && json_image_path->type == 's') {
        monster->image_path = strdup(json_image_path->string);
    }

    // ascii_path
    json_t * json_ascii_path = get_object_at_key(object, "ascii_path");
    if (json_ascii_path && json_ascii_path->type == 's') {
        monster->ascii_path = strdup(json_ascii_path->string);
    }

    return monster;
}
