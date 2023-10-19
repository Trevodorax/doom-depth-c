#include "armor.h"

void *create_armor_from_db(sqlite3_stmt *stmt) {

    armor_t *armor = malloc(sizeof(armor_t));

    if (!armor) {
        fprintf(stderr, "Failed to allocate memory for armor.\n");
        return NULL;
    }

    armor->id = sqlite3_column_int(stmt, 0);
    armor->name = strdup((char *) sqlite3_column_text(stmt, 1));
    armor->rarity = (short)sqlite3_column_int(stmt, 2);
    armor->amount = sqlite3_column_int(stmt, 3);
    armor->max_uses = sqlite3_column_int(stmt, 4);
    armor->cost = sqlite3_column_int(stmt, 5);
    armor->image_path = strdup((char *) sqlite3_column_text(stmt, 6));

    if (sqlite3_column_count(stmt) == 8) {
        armor->uses = sqlite3_column_int(stmt, 7);
    } else {
        armor->uses = armor->max_uses;
    }

    if (sqlite3_column_count(stmt) == 9) {
        armor->chosen = sqlite3_column_int(stmt, 8);
    } else {
        armor->chosen = 0;
    }

    if (sqlite3_column_count(stmt) == 10) {
        armor->id_in_inventory = sqlite3_column_int(stmt, 9);
    } else {
        armor->id_in_inventory = 0;
    }

    return armor;

}