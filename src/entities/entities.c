#include "entities.h"

list_t  *weapons = NULL;
list_t  *spells = NULL;
list_t  *armors = NULL;
list_t  *monsters = NULL;

void init_entities(sqlite3 *db) {

    weapons = create_struct_from_db(db, "SELECT * FROM WEAPON", create_weapon_from_db, WEAPON_STRUCT);
    spells = create_struct_from_db(db, "SELECT * FROM SPELL", create_spell_from_db, SPELL_STRUCT);
    armors = create_struct_from_db(db, "SELECT * FROM ARMOR", create_armor_from_db, ARMOR_STRUCT);
    monsters = create_struct_from_db(db, "SELECT * FROM MONSTER", create_monster_from_db, MONSTER_STRUCT);

}

list_t *get_weapons() {
    return weapons;
}

list_t *get_spells() {
    return spells;
}

list_t *get_armors() {
    return armors;
}

list_t *get_monsters() {
    return monsters;
}

monster_t* get_monster_by_name(char * name) {
    if (!name || !monsters) {
        return NULL;
    }

    array_node_t* current = monsters->head;
    while (current) {
        monster_t* monster = void_to_monster(current->value);
        if(!monster || !monster->name) {
            continue;
        }
        if (strcmp(monster->name, name) == 0) {
            return copy_monster(monster);
        }
        current = current->next;
    }

    return NULL;
}

armor_t * get_armor_by_id(int id) {
    if(!armors) {
        return NULL;
    }

    array_node_t * current = armors;
    while(current) {
        armor_t * armor = void_to_armor(current->value);
        if(!armor) {
            continue;
        }
        if(armor->id == id) {
            return copy_armor(armor);
        }
        current = current->next;
    }

    return NULL;
}


weapon_t * get_weapon_by_id(int id) {
    if(!weapons) {
        return NULL;
    }

    array_node_t * current = weapons;
    while(current) {
        weapon_t * weapon = void_to_weapon(current->value);
        if(!weapon) {
            continue;
        }
        if(weapon->id == id) {
            return copy_weapon(weapon);
        }
        current = current->next;
    }

    return NULL;
}
