#include "weapon.h"

void *create_weapon_from_db(sqlite3_stmt *stmt) {

    weapon_t *weapon = malloc(sizeof(weapon_t));

    if (!weapon) {
        return NULL;
    }

    weapon->id = sqlite3_column_int(stmt, 0);
    weapon->name = strdup((char *) sqlite3_column_text(stmt, 1));
    weapon->min_attack = sqlite3_column_int(stmt, 2);
    weapon->max_attack = sqlite3_column_int(stmt, 3);
    weapon->attacks_per_turn = sqlite3_column_int(stmt, 4);
    weapon->rarity = (short)sqlite3_column_int(stmt, 5);
    weapon->max_uses = sqlite3_column_int(stmt, 6);
    weapon->cost = sqlite3_column_int(stmt, 7);
    weapon->image_path = strdup((char *) sqlite3_column_text(stmt, 8));

    if (sqlite3_column_count(stmt) == 10) {
        weapon->uses = sqlite3_column_int(stmt, 9);
    } else {
        weapon->uses = weapon->max_uses;
    }

    return weapon;

}