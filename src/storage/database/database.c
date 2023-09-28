#include "database.h"
#include <stdio.h>

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

int callback(void *exists, int argc, char **argv, char **azColName) {

    int *e = (int *) exists;

    *e = 1;
    return 0;
}

int db_init(sqlite3 *db) {

    char *z_err_msg = 0;

    for (int i = 0; i < ALL_QUERIES_SIZE; i++) {

        int exists = 0;

        int rc = sqlite3_exec(db, all_tables_info[i].checkQuery, callback, &exists, &z_err_msg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", z_err_msg);
            sqlite3_free(z_err_msg);
            return rc;
        }

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

        if (db) {

            int rc = db_init(db);
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