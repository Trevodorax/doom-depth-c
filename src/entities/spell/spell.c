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