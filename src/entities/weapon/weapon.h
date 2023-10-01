#ifndef WEAPON
#define WEAPON

typedef struct {
    int id;
    char *name;
    short rarity;
    unsigned int min_attack;
    unsigned int max_attack;
    unsigned int attacks_per_turn;
    unsigned int uses;
    unsigned int max_uses;
    unsigned int cost;
} weapon_t;

weapon_t * create_weapon_from_database(char *name,
                                     short rarity,
                                     unsigned int min_attack,
                                     unsigned int max_attack,
                                     unsigned int attacks_per_turn,
                                     unsigned int uses,
                                     unsigned int max_uses,
                                     unsigned int cost);

#endif