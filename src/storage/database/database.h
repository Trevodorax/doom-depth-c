#ifndef DOOM_DEPTH_C_DATABASE_H
#define DOOM_DEPTH_C_DATABASE_H

#define MAX_TRIES 3

#include <sqlite3.h>
#include "database_queries.h"

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

/**
 * @brief Executes a given SQL query on the database.
 *
 * This function attempts to execute a given SQL query on the database.
 *
 * @param db A pointer to the SQLite3 database connection.
 * @param sql The SQL query to be executed.
 * @param errMsg An optional pointer to an error message.
 * If the execution fails and errMsg is provided, an error message is stored at *errMsg.
 *
 * @return SQLITE_OK if the query is successfully executed. Otherwise, it returns an error code.
 * @sideeffects May modify the SQLite database based on the SQL query provided.
 * @dependencies Depends on the SQLite3 library.
 * @errors May return SQLITE_ERROR or other error codes if the query execution fails.
 * @author Tom BOURLARD
 * @date 27/09/23
 */
int execute_query(sqlite3 *db, char *sql, char **z_err_msg);

/**
 * @brief Initializes the database with required tables and initial data.
 *
 * This function executes a series of SQL queries to set up the necessary tables
 * and insert initial data into them.
 *
 * @param db The SQLite3 database pointer where the tables and data will be initialized.
 * @return SQLITE_OK if the initialization is successful. Otherwise, it returns an error code.
 *
 * @sideeffects May modify the database by creating new tables or inserting data.
 * @dependencies Depends on the global ALL_QUERIES array for SQL commands.
 * @errors Possible SQLite errors due to malformed SQL commands or other SQLite-specific issues.
 * @author Tom BOURLARD
 * @date 28/09/23
 */
int db_init(sqlite3 *db);

/**
 * @brief Ensures a connection to the database, initializing it if needed.
 *
 * This function tries to establish a connection to the database. If the database does not exist,
 * it tries to create and initialize it. If it fails to ensure a connection after MAX_TRIES, it returns NULL.
 *
 * @return A pointer to the connected SQLite3 database, or NULL if connection attempts exceed MAX_TRIES.
 *
 * @sideeffects May create and initialize a new database. Modifies the database by potentially adding new tables or data.
 * @dependencies Depends on db_connect() and db_init() functions.
 * @errors Potential errors from SQLite during database connection or initialization.
 * @author Tom BOURLARD
 * @date 28/09/23
 */
sqlite3 *db_connection();

#endif //DOOM_DEPTH_C_DATABASE_H
