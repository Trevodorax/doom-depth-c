#ifndef SPELL
#define SPELL

typedef enum SpellType {
    ATTACK, DEFENSE, HEALING
} spell_type_t;

typedef struct {
    int id;
    char *name;
    spell_type_t type;
    unsigned int amount;
    unsigned int cost;
} spell_t;

spell_t *create_spell_from_database(char *name, spell_type_t type, unsigned int amount, unsigned int cost);

#endif