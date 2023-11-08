#include "spell.h"

void *create_spell_from_db(sqlite3_stmt *stmt) {

    spell_t *spell = malloc(sizeof(spell_t));
    if (spell == NULL) {
        return NULL;
    }

    spell->id = sqlite3_column_int(stmt, 0);
    spell->type = sqlite3_column_int(stmt, 1);
    spell->name = strdup((char *) sqlite3_column_text(stmt, 2));
    spell->amount = sqlite3_column_int(stmt, 3);
    spell->cost = sqlite3_column_int(stmt, 4);

    return spell;

}

spell_t *find_noob_spell(list_t * spells, int type) {
    array_node_t * current = spells->head;
    spell_t * noob_spell = NULL;
    while(current != NULL){
        spell_t * spell = (spell_t *)current->value;
            if (spell->type == type) {
                if (noob_spell == NULL) {
                    noob_spell = spell;
                } else if (spell->amount < noob_spell->amount) {
                    noob_spell = spell;
                }
            }
        current = current->next;
    }
    return noob_spell;
}

spell_t *find_spell(list_t * spells, int id) {

    array_node_t * current = spells->head;
    spell_t * spell = NULL;
    while(current != NULL){
        spell = (spell_t *)current->value;
            if (spell->id == id) {
                return spell;
            }
        current = current->next;
    }
    return NULL;

}

void free_spell(spell_t *spell) {
    if (spell) {
        free(spell->name);
        free(spell);
    }
}
