#include <stdlib.h>
#include "inventory.h"

inventory_t * create_inventory(){
    inventory_t * inv = malloc(sizeof(inventory_t));
    inv->armorsHead = NULL;
    inv->weaponsHead = NULL;
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
    inventory->armorsHead = NULL;
    inventory->weaponsHead = NULL;

    return inventory;
}

array_node_t *create_full_inventory_from_db(sqlite3 *db, int player_id) {

    char *sql = malloc(sizeof(char) * 150);
    sprintf(sql, "SELECT inventory_capacity, nb_weapons, nb_armors, nb_mana_potions, nb_health_potions FROM PLAYER WHERE id = %d", player_id);
    array_node_t *inventory = create_struct_from_db(db, sql, create_inventory_from_db, sizeof (inventory_t));

    sprintf(sql, "SELECT W.*, WI.uses, WI.chosen, WI.id FROM WEAPON W JOIN WEAPONS_IN_INVENTORY WI ON W.id = WI.weapon_id WHERE WI.player_id = %d", player_id);
    array_node_t *weapons_in_inventory = create_struct_from_db(db, sql, create_weapon_from_db, sizeof (weapon_t));

    sprintf(sql, "SELECT A.* FROM ARMOR A JOIN ARMORS_IN_INVENTORY AI ON A.id = AI.armor_id WHERE AI.player_id = %d", player_id);
    array_node_t *armors_in_inventory = create_struct_from_db(db, sql, create_armor_from_db, sizeof (armor_t));

    inventory_t *i = (inventory_t *)inventory->value;
    i->weaponsHead = weapons_in_inventory;
    i->armorsHead = armors_in_inventory;

    free(sql);
    return inventory;

}