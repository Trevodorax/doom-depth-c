#include "player.h"

player_t *create_player(char *name, array_node_t *spells) {
    player_t *player = malloc(sizeof(player_t));
    player->name = malloc(sizeof(char) * strlen(name) + 1);
    strcpy(player->name, name);

    player->hp = 200u;
    player->hp_max = 200u;
    player->mana = 100u;
    player->mana_max = 100u;
    player->xp = 0u;
    player->level = 1u;
    player->base_attack = 5u;
    player->base_defense = 2u;
    player->gold = 100u;
    player->action_points = (unsigned short) 3;

    // TODO : add one spell at least for the beginning, see with @TomBrd
    player->offensive_spell = find_noob_spell(spells, 3);
    player->defensive_spell = find_noob_spell(spells, 2);
    player->healing_spell = find_noob_spell(spells, 1);

    player->chosen_armor = NULL;
    player->chosen_weapon = NULL;

    player->inventory = create_inventory();
    player->stats = create_stats();

    return player;
}

void *create_player_from_db(sqlite3_stmt *stmt, array_node_t *spells) {

    player_t *player = malloc(sizeof(player_t));

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

    player->offensive_spell = find_spell(spells, sqlite3_column_int(stmt, 11));
    player->defensive_spell = find_spell(spells, sqlite3_column_int(stmt, 12));
    player->healing_spell = find_spell(spells, sqlite3_column_int(stmt, 13));

    array_node_t *inventory = create_full_inventory_from_db(db_connection(), player->id);
    player->inventory = (inventory_t *)inventory->value;

    char *sql = malloc(sizeof(char) * 150);
    sprintf(sql, "SELECT * FROM STATS WHERE id = (SELECT stats_id FROM PLAYER WHERE id = %d)", player->id);
    array_node_t *stats = create_struct_from_db(db_connection(), sql, create_stats_from_db, sizeof (stats_t));
    player->stats = (stats_t *)stats->value;

    player->chosen_weapon = get_chosen_weapon(player->inventory);
    player->chosen_armor = get_chosen_armor(player->inventory);

    return player;
}


unsigned int compute_xp_needed(unsigned int level) {
    int total_xp = 0;
    for (int i = 1; i <= level; ++i) {
        total_xp += 100 * i;
    }
    return (unsigned int) total_xp;
}

void level_up(player_t *player) {

}

void check_level_up(player_t *player) {
    if (compute_xp_needed(player->level + 1) <= player->xp) {
        level_up(player);
    }
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

void give_exp(player_t *player, unsigned int amount) {
    player->xp += amount;
    check_level_up(player);
}