#include "monster.h"

void *create_monster_from_db(sqlite3_stmt *stmt) {

    monster_t *monster = malloc(sizeof(monster_t));

    if (!monster) {
        fprintf(stderr, "Failed to allocate memory for monster.\n");
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

    return monster;

}

monster_t * void_to_monster(void * void_monster) {
    monster_t * monster = (monster_t *)void_monster;

    if(!monster || !monster->id || !monster->name || !monster->hp || !monster->hp_max || !monster->attack || !monster->defense || !monster->image_path) {
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
        return NULL; // memory allocation failed
    }

    // Copy simple data types
    new_monster->id = monster->id;
    new_monster->type = monster->type;
    new_monster->hp = monster->hp;
    new_monster->hp_max = monster->hp_max;
    new_monster->attack = monster->attack;
    new_monster->defense = monster->defense;

    // Deep copy name
    if (monster->name) {
        new_monster->name = strdup(monster->name);
        if (!new_monster->name) {
            free(new_monster); // Free allocated memory for new_monster
            return NULL; // memory allocation for name failed
        }
    } else {
        new_monster->name = NULL;
    }

    // Deep copy image_path
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

    return new_monster;
}
