#ifndef ARMOR
#define ARMOR

typedef struct {
    char *name;
    short rarity;
    unsigned int amount;
    unsigned int uses;
    unsigned int max_uses;
    unsigned int cost;
} Armor;

#endif