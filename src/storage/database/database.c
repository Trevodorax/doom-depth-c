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

list_t *create_struct_from_db(sqlite3 *db, const char *query, sql_to_struct_callback callback, array_type_t type) {

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    list_t *list = new_list(type);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        void *struct_ptr = callback(stmt);

        if (!struct_ptr) {
            fprintf(stderr, "Failed to allocate memory for struct.\n");
            sqlite3_finalize(stmt);
            return NULL;
        }

        append(list, struct_ptr);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    sqlite3_finalize(stmt);
    return list;

}