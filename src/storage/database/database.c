#include "database.h"
#include <stdio.h>
#include <stdlib.h>

sqlite3 *db_connect() {

    char *file_name = "../database/doomdepthc.database";
    sqlite3 *db;

    int rc = sqlite3_open(file_name, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    return db;
}

int execute_query(sqlite3 *db, const char *sql, char **z_err_msg) {

    int rc = sqlite3_exec(db, sql, 0, 0, z_err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", *z_err_msg);
        sqlite3_free(*z_err_msg);
    }

    return rc;
}

/**
 * @brief Callback function for the sqlite3_exec function.
 *
 * This function is used to check if a table exists in the database.
 *
 * @param exists // a pointer to an integer that will be set to 1 if the table exists, 0 otherwise.
 * @param argc // not used
 * @param argv // not used
 * @param azColName // not used
 * @return 0 to indicate success.
 */
int callback(void *exists, int argc, char **argv, char **azColName) {

    // exists is passed as void * and needs to be casted to int *
    // in order to be dereferenced and set to 1 if the table exists.
    int *e = (int *) exists;

    *e = 1;
    return 0;
}

int db_init(sqlite3 *db) {

    char *z_err_msg = NULL;

    for (int i = 0; i < ALL_QUERIES_SIZE; i++) {

        int exists = 0;

        // Check if the table exists
        int rc = sqlite3_exec(db, all_tables_info[i].checkQuery, callback, &exists, &z_err_msg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", z_err_msg);
            sqlite3_free(z_err_msg);
            return rc;
        }

        // If the table doesn't exist, create it and insert default values if applicable
        if (!exists) {
            rc = execute_query(db, all_tables_info[i].createQuery, &z_err_msg);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", z_err_msg);
                sqlite3_free(z_err_msg);
                return rc;
            }

            if (all_tables_info[i].insertQuery) {
                rc = execute_query(db, all_tables_info[i].insertQuery, &z_err_msg);

                if (rc != SQLITE_OK) {
                    fprintf(stderr, "SQL error: %s\n", z_err_msg);
                    sqlite3_free(z_err_msg);
                    return rc;
                }
            }
        }

    }

    return SQLITE_OK;

}

sqlite3 *db_connection() {

    int tries = 0;
    sqlite3 *db;

    while (tries < MAX_TRIES) {

        db = db_connect(); // return NULL if unable to connect

        // if db is not NULL, try to initialize it
        if (db) {

            int rc = db_init(db);
            // if initialization is successful, return the database
            if (rc == SQLITE_OK) {
                return db;
            }

            fprintf(stderr, "Unable to initialize the database.\n");
            sqlite3_close(db);
        } else {
            fprintf(stderr, "Unable to connect to the database.\n");
        }

        tries++;

    }

    return NULL;
}

array_node_t *create_struct_from_db(sqlite3 *db, const char *query, sql_to_struct_callback callback) {

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    array_node_t *head = NULL;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        void *struct_ptr = callback(stmt);

        if (!struct_ptr) {
            fprintf(stderr, "Failed to allocate memory for struct.\n");
            sqlite3_finalize(stmt);
            return NULL;
        }

        append(&head, struct_ptr, sizeof(struct_ptr));
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    sqlite3_finalize(stmt);
    return head;

}

int save_player(sqlite3 *db, player_t *player) {

    char *z_err_msg = NULL;
    sqlite3_stmt *stmt;

    // TODO: update the player's stats in the database
    // TODO: update the player's inventory in the database
        // TODO: update the player's weapons in the database : weapons_in_inventory
        // TODO: update the player's armors in the database : armors_in_inventory

        
    /* This function take in parameter the database,
     * the sql command to update the player,
     * the length of the sql command (if -1, the command is read up to the first null terminator),
     * a pointer to the statement object and
     * a pointer to the unused portion of the sql command
    */
    int rc = sqlite3_prepare_v2(db, update_player_sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }

    sqlite3_bind_int64(stmt, 1, player->hp);
    sqlite3_bind_int64(stmt, 2, player->hp_max);
    sqlite3_bind_int64(stmt, 3, player->mana);
    sqlite3_bind_int64(stmt, 4, player->mana_max);
    sqlite3_bind_int64(stmt, 5, player->xp);
    sqlite3_bind_int64(stmt, 6, player->level);
    sqlite3_bind_int64(stmt, 7, player->base_attack);
    sqlite3_bind_int64(stmt, 8, player->base_defense);
    sqlite3_bind_int64(stmt, 9, player->gold);
    sqlite3_bind_int64(stmt, 10, player->offensive_spell->id);
    sqlite3_bind_int64(stmt, 11, player->defensive_spell->id);
    sqlite3_bind_int64(stmt, 12, player->healing_spell->id);
    sqlite3_bind_int64(stmt, 13, player->inventory->capacity);
    sqlite3_bind_int64(stmt, 14, player->inventory->nb_weapons);
    sqlite3_bind_int64(stmt, 15, player->inventory->nb_armors);
    sqlite3_bind_int64(stmt, 16, player->inventory->nb_mana_potions);
    sqlite3_bind_int64(stmt, 17, player->inventory->nb_health_potions);
    sqlite3_bind_int64(stmt, 18, player->id);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;

}