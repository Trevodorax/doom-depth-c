#ifndef ARMOR
#define ARMOR

typedef struct {
    char *name;
    short rarity;
    unsigned int amount;
    unsigned int uses;
    unsigned int maxUses;
    unsigned int cost;
} Armor;

#endif