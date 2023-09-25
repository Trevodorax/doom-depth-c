#ifndef MONSTER
#define MONSTER

typedef struct {
    char *name;
    short type;
    unsigned int hp;
    unsigned int hpMax;
    unsigned int attack;
    unsigned int defense;
} Monster;

#endif