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