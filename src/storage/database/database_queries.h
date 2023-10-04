#ifndef DOOM_DEPTH_C_DATABASE_QUERIES_H
#define DOOM_DEPTH_C_DATABASE_QUERIES_H

#include <stdio.h>

#define ALL_QUERIES_SIZE 9

typedef struct Table_Info {
    const char* checkQuery;
    const char* createQuery;
    const char* insertQuery;
} Table_Info;

extern const char create_armor_table_sql[];
extern const char create_weapon_table_sql[];
extern const char create_monster_table_sql[];
extern const char create_armors_in_inventory_table_sql[];
extern const char create_weapons_in_inventory_table_sql[];
extern const char create_spell_table_sql[];
extern const char create_player_table_sql[];
extern const char create_stage_table_sql[];
extern const char create_stats_table_sql[];

extern const char insert_armor_sql[];
extern const char insert_weapon_sql[];
extern const char insert_spell_sql[];
extern const char insert_monster_sql[];

extern const char update_player_sql[];

extern Table_Info all_tables_info[];

#endif //DOOM_DEPTH_C_DATABASE_QUERIES_H
