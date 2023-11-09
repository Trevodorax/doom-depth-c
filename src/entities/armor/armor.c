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
    armor->ascii_path = strdup((char *) sqlite3_column_text(stmt, 7));

    if (sqlite3_column_count(stmt) == 9) {
        armor->uses = sqlite3_column_int(stmt, 8);
    } else {
        armor->uses = armor->max_uses;
    }

    if (sqlite3_column_count(stmt) == 10) {
        armor->chosen = sqlite3_column_int(stmt, 9);
    } else {
        armor->chosen = 0;
    }

    if (sqlite3_column_count(stmt) == 11) {
        armor->id_in_inventory = sqlite3_column_int(stmt, 10);
    } else {
        armor->id_in_inventory = 0;
    }

    return armor;

}

armor_t* copy_armor(const armor_t* armor) {
    if (!armor) {
        return NULL;
    }

    armor_t* new_armor = (armor_t*) malloc(sizeof(armor_t));
    if (!new_armor) {
        return NULL;
    }

    new_armor->chosen = armor->chosen;
    new_armor->id_in_inventory = armor->id_in_inventory;
    new_armor->amount = armor->amount;
    new_armor->max_uses = armor->max_uses;
    new_armor->uses = armor->uses;
    new_armor->rarity = armor->rarity;
    new_armor->cost = armor->cost;
    new_armor->amount = armor->amount;
    new_armor->id = armor->id;

    // deep copy pointers
    if (armor->name) {
        new_armor->name = strdup(armor->name);
        if (!new_armor->name) {
            free(new_armor); // Free allocated memory for new_armor
            return NULL; // memory allocation for name failed
        }
    } else {
        new_armor->name = NULL;
    }

    if (armor->image_path) {
        new_armor->image_path = strdup(armor->image_path);
        if (!new_armor->image_path) {
            free(new_armor->name); // Free allocated memory for name
            free(new_armor); // Free allocated memory for new_armor
            return NULL; // memory allocation for image_path failed
        }
    } else {
        new_armor->image_path = NULL;
    }

    if (armor->ascii_path) {
        new_armor->ascii_path = strdup(armor->ascii_path);
        if (!new_armor->ascii_path) {
            free(new_armor); // Free allocated memory for new_armor
            return NULL; // memory allocation for ascii_path failed
        }
    } else {
        new_armor->ascii_path = NULL;
    }

    return new_armor;
}

armor_t *void_to_armor(void *void_armor) {
    armor_t *armor = (armor_t *) void_armor;

    if (!armor || !armor->name || !armor->image_path || !armor->ascii_path) {
        return NULL;
    }

    return armor;
}

void free_armor(armor_t *armor) {
    if (armor) {
        free(armor->name);
        free(armor->image_path);
        free(armor->ascii_path);
        free(armor);
    }
}
