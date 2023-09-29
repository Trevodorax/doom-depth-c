#ifndef ARMOR
#define ARMOR

typedef struct {
    char *name;
    short rarity;
    unsigned int amount;
    unsigned int uses;
    unsigned int max_uses;
    unsigned int cost;
} armor_t;

armor_t *create_armor_from_database(char *name,
                                    short rarity,
                                    unsigned int amount,
                                    unsigned int uses,
                                    unsigned int max_uses,
                                    unsigned int cost);

#endif