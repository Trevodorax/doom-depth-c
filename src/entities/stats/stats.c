#include "stats.h"

stats_t * create_stats(){
    stats_t * stats = malloc(sizeof(stats_t));
    stats->damages_dealt = 0u;
    stats->health_healed = 0u;
    stats->nb_deaths = 0u;
    stats->nb_monsters_killed = 0u;
    return stats;
}

void *create_stats_from_db(sqlite3_stmt *stmt) {

    stats_t *stats = malloc(sizeof(stats_t));

    if (!stats) {
        fprintf(stderr, "Failed to allocate memory for stats.\n");
        return NULL;
    }

    stats->id = sqlite3_column_int(stmt, 0);
    stats->nb_monsters_killed = sqlite3_column_int(stmt, 1);
    stats->nb_deaths = sqlite3_column_int(stmt, 2);
    stats->damages_dealt = sqlite3_column_int(stmt, 3);
    stats->health_healed = sqlite3_column_int(stmt, 4);

    return stats;

}

int create_stats_in_db(sqlite3 *db, stats_t *stats) {

    char *z_err_msg = NULL;
    sqlite3_stmt *stmt;

    char *sql = malloc(sizeof(char) * 150);
    sprintf(sql, "INSERT INTO STATS (nb_monsters_killed, nb_deaths, damages_dealt, health_healed) VALUES (%d, %d, %d, %d)", stats->nb_monsters_killed, stats->nb_deaths, stats->damages_dealt, stats->health_healed);

    int rc = sqlite3_exec(db, sql, NULL, 0, &z_err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return -1;
    }

    free(sql);
    return (int) sqlite3_last_insert_rowid(db);

}

int save_stats(sqlite3 *db, stats_t *stats, int player_id) {

    char *z_err_msg = NULL;
    sqlite3_stmt *stmt;

    char *sql = malloc(sizeof(char) * 150);
    sprintf(sql, "UPDATE STATS SET nb_monsters_killed = %d, nb_deaths = %d, damages_dealt = %d, health_healed = %d WHERE id = %d", stats->nb_monsters_killed, stats->nb_deaths, stats->damages_dealt, stats->health_healed, player_id);

    int rc = sqlite3_exec(db, sql, NULL, 0, &z_err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return 1;
    }

    free(sql);
    return 0;

}

void free_stats(stats_t *stats) {
    if (stats) {
        free(stats);
    }
}
