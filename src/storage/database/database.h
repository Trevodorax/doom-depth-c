#ifndef DOOM_DEPTH_C_DATABASE_H
#define DOOM_DEPTH_C_DATABASE_H

#define MAX_TRIES 3

#include <sqlite3.h>
#include "database_queries.h"
#include "../../entities/player/player.h"
#include "../../utils/array.h"

// pointer to a function that takes a sqlite3_stmt and returns a void pointer
typedef void* (*sql_to_struct_callback)(sqlite3_stmt *stmt);

/**
 * @brief Connects to the SQLite database or returns NULL if it fails.
 *
 * This function tries to open a connection to the SQLite database.
 *
 * @return A pointer to the SQLite database, or NULL on failure.
 * @sideeffects None.
 * @dependencies Depends on the SQLite3 library.
 * @errors Returns NULL if unable to connect to the database.
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
 */
int execute_query(sqlite3 *db, const char *sql, char **z_err_msg);

/**
 * @brief Initializes the database by creating necessary tables and inserting default values.
 *
 * This function iterates over the predefined table information in `all_tables_info`.
 * For each table:
 * - It checks if the table exists in the database.
 * - If the table doesn't exist, it creates the table and, if applicable, inserts default values into the table.
 *
 * @param db    A pointer to the SQLite3 database connection.
 * @return      SQLITE_OK if the initialization is successful; an error code otherwise.
 *
 * @sideeffects May modify the database by creating new tables or inserting data.
 * @dependencies Depends on the global ALL_QUERIES array for SQL commands.
 * @errors Possible SQLite errors due to malformed SQL commands or other SQLite-specific issues.
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
 */
sqlite3 *db_connection();

/**
 * @brief Creates a struct from the database using a given query and callback function.
 *
 * This function executes a given SQL query on the database and creates a struct from the result.
 *
 * @param db A pointer to the SQLite3 database connection.
 * @param query The SQL query to be executed.
 * @param callback A pointer to a function that takes a sqlite3_stmt and returns a void pointer.
 * @param struct_size The size of the struct to be created.
 *
 * @return A pointer to the struct created from the database, or NULL if the query execution fails.
 * @sideeffects May modify the SQLite database by inserting new data.
 * @dependencies Depends on the SQLite3 library.
 * @errors May return SQLITE_ERROR or other error codes if the query execution fails.
 */
array_node_t *create_struct_from_db(sqlite3 *db, const char *query, sql_to_struct_callback callback, size_t struct_size);

/**
 * @brief Saves the player's data to the database.
 *
 * This function saves the player's data to the database.
 *
 * @param db A pointer to the SQLite3 database connection.
 * @param player A pointer to the player whose data is to be saved.
 *
 * @return SQLITE_OK if the player's data is successfully saved. Otherwise, it returns an error code.
 * @sideeffects May modify the SQLite database by inserting new data.
 * @dependencies Depends on the SQLite3 library.
 * @errors May return SQLITE_ERROR or other error codes if the query execution fails.
 */
int save_player(sqlite3 *db, player_t *player);

#endif //DOOM_DEPTH_C_DATABASE_H
