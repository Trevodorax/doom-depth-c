#ifndef WEAPON
#define WEAPON

typedef struct {
    char *name;
    short rarity;
    unsigned int minAttack;
    unsigned int maxAttack;
    unsigned int attacksPerTurn;
    unsigned int uses;
    unsigned int maxUses;
    unsigned int cost;
} Weapon;

#endif