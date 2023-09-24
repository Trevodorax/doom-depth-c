#ifndef WEAPON
#define WEAPON

typedef struct {
    char *name;
    short rarity;
    int minAttack;
    int maxAttack;
    int attacksPerTurn;
    int uses;
    int maxUses;
    int cost;
} Weapon;

#endif