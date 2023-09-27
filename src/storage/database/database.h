#ifndef DOOM_DEPTH_C_DATABASE_H
#define DOOM_DEPTH_C_DATABASE_H

#include <sqlite3.h>

/**
 * @brief Connects to the SQLite database or returns NULL if it fails.
 *
 * This function tries to open a connection to the SQLite database.
 *
 * @return A pointer to the SQLite database, or NULL on failure.
 * @sideeffects None.
 * @dependencies Depends on the SQLite3 library.
 * @errors Returns NULL if unable to connect to the database.
 * @author Tom BOURLARD
 * @date 27/09/23
 */
sqlite3 *db_connect();

#endif //DOOM_DEPTH_C_DATABASE_H
