#ifndef MONSTER
#define MONSTER

typedef struct {
    char *name;
    short type;
    int hp;
    int hpMax;
    int attack;
    int defense;
} Monster;

#endif