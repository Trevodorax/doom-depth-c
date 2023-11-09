#include "player.h"

unsigned int compute_xp_needed(unsigned int level) {
    unsigned int total_xp = 0;
    for (int i = 1; i <= level; ++i) {
        total_xp += 20 * i;
    }
    return total_xp;
}

void check_level_up(player_t *player) {
    return compute_xp_needed(player->level) <= player->xp;
}

void give_exp(player_t *player, unsigned int amount) {
    player->xp += amount;
}

unsigned int heal_player(player_t *player, unsigned int amount) {
    if (player->hp_max < player->hp + amount) {
        unsigned int diff = player->hp_max - player->hp;
        player->hp = player->hp_max;
        return diff;
    } else {
        player->hp += amount;
        return amount;
    }
}

void heal_mana(player_t *player, unsigned int amount) {
    if (player->mana_max < player->mana + amount) {
        player->mana = player->mana_max;
    } else {
        player->mana += amount;
    }
}

player_t *create_player(char *name) {
    player_t *player = malloc(sizeof(player_t));
    player->name = malloc(sizeof(char) * strlen(name) + 1);
    strcpy(player->name, name);

    list_t *spells = get_spells();

    player->hp_max = 100u;
    player->hp = player->hp_max;
    player->mana_max = 100u;
    player->mana = player->mana_max;
    player->xp = 0u;
    player->level = 1u;
    player->base_attack = 5u;
    player->base_defense = 2u;
    player->gold = 100u;
    player->max_action_points = (unsigned short) 6;
    player->action_points = player->max_action_points;
    player->is_defending = false;

    player->offensive_spell = find_noob_spell(spells, 3);
    player->defensive_spell = find_noob_spell(spells, 2);
    player->healing_spell = find_noob_spell(spells, 1);

    player->chosen_armor = NULL;
    player->chosen_weapon = NULL;

    player->inventory = create_inventory();
    player->stats = create_stats();

    player->current_map = strdup("map_1.json");

    return player;
}

void create_player_in_db(player_t *player) {
    sqlite3 *db = db_connect();
    char *z_err_msg = NULL;
    sqlite3_stmt *stmt;

    int stats_id = create_stats_in_db(db, player->stats);
    if (stats_id == -1) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return;
    }

    sqlite3_prepare_v2(db, create_new_player_sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, player->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, player->current_map, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, player->hp);
    sqlite3_bind_int64(stmt, 4, player->hp_max);
    sqlite3_bind_int64(stmt, 5, player->mana);
    sqlite3_bind_int64(stmt, 6, player->mana_max);
    sqlite3_bind_int64(stmt, 7, player->xp);
    sqlite3_bind_int64(stmt, 8, player->level);
    sqlite3_bind_int64(stmt, 9, player->base_attack);
    sqlite3_bind_int64(stmt, 10, player->base_defense);
    sqlite3_bind_int64(stmt, 11, player->gold);
    sqlite3_bind_int64(stmt, 12, player->offensive_spell->id);
    sqlite3_bind_int64(stmt, 13, player->defensive_spell->id);
    sqlite3_bind_int64(stmt, 14, player->healing_spell->id);
    sqlite3_bind_int64(stmt, 15, stats_id);
    sqlite3_bind_int64(stmt, 16, player->inventory->capacity);
    sqlite3_bind_int64(stmt, 17, player->inventory->nb_weapons);
    sqlite3_bind_int64(stmt, 18, player->inventory->nb_armors);
    sqlite3_bind_int64(stmt, 19, player->inventory->nb_mana_potions);
    sqlite3_bind_int64(stmt, 20, player->inventory->nb_health_potions);
    sqlite3_bind_int64(stmt, 21, player->action_points);
    sqlite3_bind_int64(stmt, 22, player->max_action_points);

    int rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

}

void *create_player_from_db(sqlite3_stmt *stmt) {

    player_t *player = malloc(sizeof(player_t));
    list_t *spells = get_spells();

    if (!player) {
        fprintf(stderr, "Failed to allocate memory for player.\n");
        return NULL;
    }

    player->id = sqlite3_column_int(stmt, 0);
    player->name = strdup((char *) sqlite3_column_text(stmt, 1));
    player->hp = sqlite3_column_int(stmt, 2);
    player->hp_max = sqlite3_column_int(stmt, 3);
    player->mana = sqlite3_column_int(stmt, 4);
    player->mana_max = sqlite3_column_int(stmt, 5);
    player->gold = sqlite3_column_int(stmt, 6);
    player->xp = sqlite3_column_int(stmt, 7);
    player->level = sqlite3_column_int(stmt, 8);
    player->base_attack = sqlite3_column_int(stmt, 9);
    player->base_defense = sqlite3_column_int(stmt, 10);
    player->action_points = sqlite3_column_int(stmt, 11);
    player->max_action_points = sqlite3_column_int(stmt, 12);
    player->is_defending = false;

    player->offensive_spell = find_spell(spells, sqlite3_column_int(stmt, 13));
    player->defensive_spell = find_spell(spells, sqlite3_column_int(stmt, 14));
    player->healing_spell = find_spell(spells, sqlite3_column_int(stmt, 15));
    player->current_map = strdup((char *) sqlite3_column_text(stmt, 16));

    list_t *inventory = create_full_inventory_from_db(db_connection(), player->id);
    player->inventory = (inventory_t *) inventory->head->value;

    char *sql = malloc(sizeof(char) * 150);
    sprintf(sql, "SELECT * FROM STATS WHERE id = (SELECT stats_id FROM PLAYER WHERE id = %d)", player->id);
    list_t *stats = create_struct_from_db(db_connection(), sql, create_stats_from_db, sizeof(stats_t));
    player->stats = (stats_t *) stats->head->value;

    player->chosen_weapon = get_chosen_weapon(player->inventory);
    player->chosen_armor = get_chosen_armor(player->inventory);

    return player;
}

int save_player(sqlite3 *db, player_t *player) {

    char *z_err_msg = NULL;
    sqlite3_stmt *stmt;

    int rc = save_stats(db, player->stats, player->id);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }

    rc = save_inventory(db, player->inventory, player->id);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }


    /* This function take in parameter the database,
     * the sql command to update the player,
     * the length of the sql command (if -1, the command is read up to the first null terminator),
     * a pointer to the statement object and
     * a pointer to the unused portion of the sql command
    */
    rc = sqlite3_prepare_v2(db, update_player_sql, -1, &stmt, NULL);

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

    if (player->offensive_spell != NULL) {
        sqlite3_bind_int64(stmt, 10, player->offensive_spell->id);
    } else {
        sqlite3_bind_null(stmt, 10);
    }

    if (player->defensive_spell != NULL) {
        sqlite3_bind_int64(stmt, 11, player->defensive_spell->id);
    } else {
        sqlite3_bind_null(stmt, 11);
    }

    if (player->healing_spell != NULL) {
        sqlite3_bind_int64(stmt, 12, player->healing_spell->id);
    } else {
        sqlite3_bind_null(stmt, 12);
    }

    sqlite3_bind_int64(stmt, 13, player->inventory->capacity);
    sqlite3_bind_int64(stmt, 14, player->inventory->nb_weapons);
    sqlite3_bind_int64(stmt, 15, player->inventory->nb_armors);
    sqlite3_bind_int64(stmt, 16, player->inventory->nb_mana_potions);
    sqlite3_bind_int64(stmt, 17, player->inventory->nb_health_potions);
    sqlite3_bind_int64(stmt, 18, player->action_points);
    sqlite3_bind_int64(stmt, 19, player->max_action_points);
    sqlite3_bind_text(stmt, 20, player->current_map, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 21, player->id);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;

}

void free_player(player_t *player) {
    if (player) {
        free(player->name);
        free_spell(player->offensive_spell);
        free_spell(player->defensive_spell);
        free_spell(player->healing_spell);
        free_armor(player->chosen_armor);
        free_weapon(player->chosen_weapon);
        free_inventory(player->inventory);
        free_stats(player->stats);
        free(player);
    }
}

list_t *get_players_from_db(sqlite3 *db) {

    char sql_request[100] = "SELECT id, name FROM PLAYER LIMIT 3";

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql_request, -1, &stmt, NULL);
    list_t *players = new_list(PLAYER_STRUCT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        player_t *player = malloc(sizeof(player_t));
        player->id = sqlite3_column_int(stmt, 0);
        player->name = strdup((char *) sqlite3_column_text(stmt, 1));
        append(players, player);
    }

    return players;
}

void player_state_checkpoint(player_t * player, bool save) {
    static unsigned int hp = -1;
    static unsigned int mana = -1;
    static unsigned int xp = -1;
    static unsigned int level = -1;

    if (!save) {
        if(hp == -1 || mana == -1 || xp == -1 || level == -1) {
            return;
        }

        player->hp = hp;
        player->mana = mana;
        player->xp = xp;
        player->level = level;
    }
    if (save) {
        hp = player->hp;
        mana = player->mana;
        xp = player->xp;
        level = player->level;
    }
}
