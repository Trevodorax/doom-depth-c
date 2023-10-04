#ifndef MONSTER
#define MONSTER

typedef struct {
    int id;
    char *name;
    short type;
    unsigned int hp;
    unsigned int hp_max;
    unsigned int attack;
    unsigned int defense;
} monster_t;

monster_t *create_monster_from_database(char *name,
                                        short type,
                                        unsigned int hp,
                                        unsigned int hp_max,
                                        unsigned int attack,
                                        unsigned int defense);

#endif