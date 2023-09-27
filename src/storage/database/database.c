#include "database.h"
#include <stdio.h>

sqlite3 *db_connect() {

    char *file_name = "doomdepthc.database";
    sqlite3 *db;

    int rc = sqlite3_open(file_name, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    return db;
}

int execute_query(sqlite3 *db, char *sql, char **z_err_msg) {

    int rc = sqlite3_exec(db, sql, 0, 0, z_err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", *z_err_msg);
        sqlite3_free(*z_err_msg);
    }

    return rc;
}