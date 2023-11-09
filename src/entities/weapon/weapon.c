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
    weapon->ascii_path = strdup((char *) sqlite3_column_text(stmt, 9));

    if (sqlite3_column_count(stmt) == 11) {
        weapon->uses = sqlite3_column_int(stmt, 10);
    } else {
        weapon->uses = weapon->max_uses;
    }

    if (sqlite3_column_count(stmt) == 12) {
        weapon->chosen = sqlite3_column_int(stmt, 11);
    } else {
        weapon->chosen = 0;
    }

    if (sqlite3_column_count(stmt) == 13) {
        weapon->id_in_inventory = sqlite3_column_int(stmt, 12);
    } else {
        weapon->id_in_inventory = 0;
    }

    return weapon;
}

void free_weapon(weapon_t *weapon) {
    if (weapon) {
        free(weapon->name);
        free(weapon->image_path);
        free(weapon->ascii_path);
        free(weapon);
    }
}

weapon_t* copy_weapon(const weapon_t* weapon) {
    if (!weapon) {
        return NULL;
    }

    weapon_t* new_weapon = (weapon_t*) malloc(sizeof(weapon_t));
    if (!new_weapon) {
        return NULL;
    }

    new_weapon->chosen = weapon->chosen;
    new_weapon->id_in_inventory = weapon->id_in_inventory;
    new_weapon->min_attack = weapon->min_attack;
    new_weapon->max_attack = weapon->max_attack;
    new_weapon->attacks_per_turn = weapon->attacks_per_turn;
    new_weapon->max_uses = weapon->max_uses;
    new_weapon->uses = weapon->uses;
    new_weapon->rarity = weapon->rarity;
    new_weapon->cost = weapon->cost;
    new_weapon->id = weapon->id;

    // deep copy pointers
    if (weapon->name) {
        new_weapon->name = strdup(weapon->name);
        if (!new_weapon->name) {
            free(new_weapon); // Free allocated memory for new_weapon
            return NULL; // memory allocation for name failed
        }
    } else {
        new_weapon->name = NULL;
    }

    if (weapon->image_path) {
        new_weapon->image_path = strdup(weapon->image_path);
        if (!new_weapon->image_path) {
            free(new_weapon->name); // Free allocated memory for name
            free(new_weapon); // Free allocated memory for new_weapon
            return NULL; // memory allocation for image_path failed
        }
    } else {
        new_weapon->image_path = NULL;
    }

    if (weapon->ascii_path) {
        new_weapon->ascii_path = strdup(weapon->ascii_path);
        if (!new_weapon->ascii_path) {
            free(new_weapon); // Free allocated memory for new_weapon
            return NULL; // memory allocation for ascii_path failed
        }
    } else {
        new_weapon->ascii_path = NULL;
    }

    return new_weapon;
}

weapon_t *void_to_weapon(void *void_weapon) {
    weapon_t *weapon = (weapon_t *) void_weapon;

    if (!weapon || !weapon->name || !weapon->image_path || !weapon->ascii_path) {
        return NULL;
    }

    return weapon;
}
