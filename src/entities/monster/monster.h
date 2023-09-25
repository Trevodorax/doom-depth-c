#ifndef MONSTER
#define MONSTER

typedef struct {
    char *name;
    short type;
    unsigned int hp;
    unsigned int hp_max;
    unsigned int attack;
    unsigned int defense;
} Monster;

#endif