#include <stdlib.h>
#include "inventory.h"

inventory_t * create_inventory(){
    inventory_t * inv = malloc(sizeof(inventory_t));
    inv->armors_head = NULL;
    inv->weapons_head = NULL;
    inv->nb_armors = 0u;
    inv->nb_weapons = 0u;
    inv->nb_health_potions = 1u;
    inv->nb_mana_potions = 0u;
    inv->capacity = 10u;
    return inv;
}

void *create_inventory_from_db(sqlite3_stmt *stmt) {

    inventory_t *inventory = malloc(sizeof(inventory_t));

    if (!inventory) {
        fprintf(stderr, "Failed to allocate memory for inventory.\n");
        return NULL;
    }

    inventory->capacity = sqlite3_column_int(stmt, 0);
    inventory->nb_weapons = sqlite3_column_int(stmt, 1);
    inventory->nb_armors = sqlite3_column_int(stmt, 2);
    inventory->nb_mana_potions = sqlite3_column_int(stmt, 3);
    inventory->nb_health_potions = sqlite3_column_int(stmt, 4);
    inventory->armors_head = NULL;
    inventory->weapons_head = NULL;

    return inventory;
}

array_node_t *create_full_inventory_from_db(sqlite3 *db, int player_id) {

    char *sql = malloc(sizeof(char) * 150);
    sprintf(sql, "SELECT inventory_capacity, nb_weapons, nb_armors, nb_mana_potions, nb_health_potions FROM PLAYER WHERE id = %d", player_id);
    array_node_t *inventory = create_struct_from_db(db, sql, create_inventory_from_db, sizeof (inventory_t));

    sprintf(sql, "SELECT W.*, WI.uses, WI.chosen, WI.id FROM WEAPON W JOIN WEAPONS_IN_INVENTORY WI ON W.id = WI.weapon_id WHERE WI.player_id = %d", player_id);
    array_node_t *weapons_in_inventory = create_struct_from_db(db, sql, create_weapon_from_db, sizeof (weapon_t));

    sprintf(sql, "SELECT A.*, AI.uses, AI.chosen, AI.id FROM ARMOR A JOIN ARMORS_IN_INVENTORY AI ON A.id = AI.armor_id WHERE AI.player_id = %d", player_id);
    array_node_t *armors_in_inventory = create_struct_from_db(db, sql, create_armor_from_db, sizeof (armor_t));

    inventory_t *i = (inventory_t *)inventory->value;
    i->weapons_head = weapons_in_inventory;
    i->armors_head = armors_in_inventory;

    free(sql);
    return inventory;

}

weapon_t *get_chosen_weapon(inventory_t *inventory){

    array_node_t * current_node = inventory->weapons_head;
    while(current_node != NULL){
        weapon_t * weapon = (weapon_t *) current_node->value;
        if(weapon->chosen == 1){
            return weapon;
        }
        current_node = current_node->next;
    }

    return NULL;

}

armor_t *get_chosen_armor(inventory_t *inventory) {

    array_node_t *current_node = inventory->armors_head;
    while (current_node != NULL) {
        armor_t *armor = (armor_t *) current_node->value;
        if (armor->chosen == 1) {
            return armor;
        }
        current_node = current_node->next;
    }

    return NULL;
}

int save_inventory(sqlite3 *db, inventory_t *inventory, int player_id) {

    char *z_err_msg = NULL;
    sqlite3_stmt *stmt;

    // Delete all items from the tables WEAPONS_IN_INVENTORY and ARMORS_IN_INVENTORY for the given player_id
    char *delete_weapons_in_inventory_sql = "DELETE FROM WEAPONS_IN_INVENTORY WHERE PLAYER_ID = ?";
    int rc = sqlite3_prepare_v2(db, delete_weapons_in_inventory_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }
    sqlite3_bind_int64(stmt, 1, player_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }
    sqlite3_finalize(stmt);

    char *delete_armors_in_inventory_sql = "DELETE FROM ARMORS_IN_INVENTORY WHERE PLAYER_ID = ?";
    rc = sqlite3_prepare_v2(db, delete_armors_in_inventory_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }
    sqlite3_bind_int64(stmt, 1, player_id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }
    sqlite3_finalize(stmt);

    // Insert all items from the inventory into the tables WEAPONS_IN_INVENTORY and ARMORS_IN_INVENTORY for the given player_id
    char *insert_weapons_in_inventory_sql = "INSERT INTO WEAPONS_IN_INVENTORY (player_id, weapon_id, uses, chosen) VALUES (?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insert_weapons_in_inventory_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }

    array_node_t *current_node = inventory->weapons_head;
    while (current_node != NULL) {
        weapon_t *weapon = (weapon_t *) current_node->value;
        sqlite3_bind_int64(stmt, 1, player_id);
        sqlite3_bind_int64(stmt, 2, weapon->id);
        sqlite3_bind_int64(stmt, 3, weapon->uses);
        sqlite3_bind_int64(stmt, 4, weapon->chosen);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", z_err_msg);
            sqlite3_free(z_err_msg);
            return rc;
        }
        sqlite3_reset(stmt);
        current_node = current_node->next;
    }

    sqlite3_finalize(stmt);

    char *insert_armors_in_inventory_sql = "INSERT INTO ARMORS_IN_INVENTORY (player_id, armor_id, uses, chosen) VALUES (?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insert_armors_in_inventory_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", z_err_msg);
        sqlite3_free(z_err_msg);
        return rc;
    }

    current_node = inventory->armors_head;
    while (current_node != NULL) {
        armor_t *armor = (armor_t *) current_node->value;
        sqlite3_bind_int64(stmt, 1, player_id);
        sqlite3_bind_int64(stmt, 2, armor->id);
        sqlite3_bind_int64(stmt, 3, armor->uses);
        sqlite3_bind_int64(stmt, 4, armor->chosen);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", z_err_msg);
            sqlite3_free(z_err_msg);
            return rc;
        }
        sqlite3_reset(stmt);
        current_node = current_node->next;
    }

    sqlite3_finalize(stmt);

    return SQLITE_OK;
}

void free_inventory(inventory_t * inventory) {
    if (inventory) {
        array_node_t *current = inventory->armors_head;
        while (current) {
            array_node_t *temp = current;
            current = current->next;
            free_armor((armor_t *)temp->value);
            free_array_node(temp);
        }

        current = inventory->weapons_head;
        while (current) {
            array_node_t *temp = current;
            current = current->next;
            free_weapon((weapon_t *)temp->value);
            free_array_node(temp);
        }

        free(inventory);
    }
}