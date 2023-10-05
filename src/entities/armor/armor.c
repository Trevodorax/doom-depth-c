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
    armor->uses = armor->max_uses;
    armor->cost = sqlite3_column_int(stmt, 5);
    armor->image_path = strdup((char *) sqlite3_column_text(stmt, 6));

    return armor;

}