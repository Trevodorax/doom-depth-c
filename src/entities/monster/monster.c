#include "monster.h"

array_node_t *create_monsters_from_db(sqlite3 *db) {

    const char *query = "SELECT * FROM MONSTER";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    array_node_t *head = NULL;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        monster_t *monster = malloc(sizeof(monster_t));

        if (!monster) {
            fprintf(stderr, "Failed to allocate memory for monster.\n");
            sqlite3_finalize(stmt);
            return NULL;
        }

        monster->id = sqlite3_column_int(stmt, 0);
        monster->type = (short)sqlite3_column_int(stmt, 1);
        monster->name = strdup((char *) sqlite3_column_text(stmt, 2));
        monster->hp = sqlite3_column_int(stmt, 3);
        monster->hp_max = sqlite3_column_int(stmt, 4);
        monster->attack = sqlite3_column_int(stmt, 5);
        monster->defense = sqlite3_column_int(stmt, 6);
        monster->image = strdup((char *) sqlite3_column_text(stmt, 7));

        append(&head, monster, sizeof(monster_t));
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    sqlite3_finalize(stmt);
    return head;

}