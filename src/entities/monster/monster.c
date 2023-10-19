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