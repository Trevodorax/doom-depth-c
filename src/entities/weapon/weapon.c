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
    weapon->rarity = sqlite3_column_int(stmt, 5);
    weapon->max_uses = sqlite3_column_int(stmt, 6);
    weapon->uses = weapon->max_uses;
    weapon->cost = sqlite3_column_int(stmt, 7);
    weapon->image_path = strdup((char *) sqlite3_column_text(stmt, 8));

    return weapon;

}