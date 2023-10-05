#ifndef ARMOR
#define ARMOR

#include "../../utils/array.h"
#include <sqlite3.h>

typedef struct {
    int id;
    char *name;
    short rarity;
    unsigned int amount;
    unsigned int uses;
    unsigned int max_uses;
    unsigned int cost;
    char *image_path;
} armor_t;

#endif