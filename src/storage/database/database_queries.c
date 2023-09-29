#include "database_queries.h"

const char create_armor_table_sql[] =
        "CREATE TABLE IF NOT EXISTS ARMOR ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "amount INTEGER NOT NULL,"
        "rarity INTEGER NOT NULL,"
        "max_uses INTEGER NOT NULL,"
        "uses INTEGER NOT NULL,"
        "cost INTEGER NOT NULL,"
        "image TEXT NOT NULL"
        ");";

const char create_weapon_table_sql[] =
        "CREATE TABLE IF NOT EXISTS WEAPON ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "min_attack INTEGER NOT NULL,"
        "max_attack INTEGER NOT NULL,"
        "attacks_per_turn INTEGER NOT NULL,"
        "rarity INTEGER NOT NULL,"
        "max_uses INTEGER NOT NULL,"
        "uses INTEGER NOT NULL,"
        "cost INTEGER NOT NULL,"
        "image TEXT NOT NULL"
        ");";

const char create_monster_table_sql[] =
        "CREATE TABLE IF NOT EXISTS MONSTER ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "type INTEGER NOT NULL,"
        "name TEXT NOT NULL,"
        "hp_max INTEGER NOT NULL,"
        "hp INTEGER NOT NULL,"
        "attack INTEGER NOT NULL,"
        "defense INTEGER NOT NULL,"
        "image TEXT NOT NULL"
        ");";

const char create_inventory_table_sql[] =
        "CREATE TABLE IF NOT EXISTS INVENTORY ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "capacity INTEGER NOT NULL,"
        "nb_weapons INTEGER NOT NULL,"
        "nb_armors INTEGER NOT NULL,"
        "nb_mana_potions INTEGER NOT NULL,"
        "nb_health_potions INTEGER NOT NULL"
        ");";

const char create_armors_in_inventory_table_sql[] =
        "CREATE TABLE IF NOT EXISTS ARMORS_IN_INVENTORY ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "inventory_id INTEGER NOT NULL REFERENCES INVENTORY,"
        "armor_id INTEGER NOT NULL REFERENCES ARMOR"
        ");";

const char create_weapons_in_inventory_table_sql[] =
        "CREATE TABLE IF NOT EXISTS WEAPONS_IN_INVENTORY ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "inventory_id INTEGER NOT NULL REFERENCES INVENTORY,"
        "weapon_id INTEGER NOT NULL REFERENCES WEAPON"
        ");";

const char create_spell_table_sql[] =
        "CREATE TABLE IF NOT EXISTS SPELL ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "type INTEGER NOT NULL,"
        "name TEXT NOT NULL,"
        "amount INTEGER NOT NULL,"
        "cost INTEGER NOT NULL"
        ");";

const char create_player_table_sql[] =
        "CREATE TABLE IF NOT EXISTS PLAYER ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "hp INTEGER NOT NULL,"
        "hp_max INTEGER NOT NULL,"
        "mana INTEGER NOT NULL,"
        "mana_max INTEGER NOT NULL,"
        "gold INTEGER NOT NULL,"
        "xp INTEGER NOT NULL,"
        "level INTEGER NOT NULL,"
        "base_attack INTEGER NOT NULL,"
        "base_defense INTEGER NOT NULL,"
        "offensive_spell_id INTEGER NOT NULL REFERENCES SPELL,"
        "defensive_spell_id INTEGER NOT NULL REFERENCES SPELL,"
        "healing_spell_id INTEGER NOT NULL REFERENCES SPELL,"
        "inventory_id INTEGER NOT NULL REFERENCES INVENTORY,"
        "chosen_weapon_id INTEGER NOT NULL REFERENCES WEAPON,"
        "chosen_armor_id INTEGER NOT NULL REFERENCES ARMOR,"
        "stats_id INTEGER NOT NULL REFERENCES STATS"
        ");";

const char create_stage_table_sql[] =
        "CREATE TABLE IF NOT EXISTS STAGE ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nb_enemies INTEGER NOT NULL,"
        "enemy_chances_to_appear TEXT NOT NULL"
        ");";


const char create_stats_table_sql[] =
        "CREATE TABLE IF NOT EXISTS STATS ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nb_monsters_killed INTEGER NOT NULL,"
        "nb_deaths INTEGER NOT NULL,"
        "damages_dealt INTEGER NOT NULL,"
        "health_healed INTEGER NOT NULL,"
        "max_level_reached INTEGER NOT NULL"
        ");";


const char insert_armor_sql[] =
        "INSERT INTO ARMOR (name, amount, rarity, max_uses, uses, cost, image)"
        "VALUES ('Leather Armor', 1, 1, 10, 10, 10, '../../../assets/leather_armor.png'),"
        "('Chainmail', 1, 1, 20, 20, 20, '../../../assets/chainmail.png'),"
        "('Plate Armor', 1, 2, 30, 30, 30, '../../../assets/plate_armor.png'),"
        "('Dragon Armor', 1, 2, 40, 40, 40, '../../../assets/dragon_armor.png'),"
        "('God Armor', 1, 3, 50, 50, 50, '../../../assets/god_armor.png');";

const char insert_weapon_sql[] =
        "INSERT INTO WEAPON (name, min_attack, max_attack, attacks_per_turn, rarity, max_uses, uses, cost, image)"
        "VALUES ('Butter Knife', 1, 2, 1, 1, 10, 10, 3, '../../../assets/butter_knife.png'),"
        "('Compass', 2, 4, 1, 1, 10, 10, 5, '../../../assets/compass.png'),"
        "('Wooden Sword', 3, 6, 1, 2, 10, 10, 10, '../../../assets/wooden_sword.png'),"
        "('Lightsaber', 4, 8, 1, 2, 10, 10, 15, '../../../assets/lightsaber.png'),"
        "('Flip-Flops', 5, 10, 1, 3, 10, 10, 20, '../../../assets/flip_flops.png'),"
        "('Wand', 6, 12, 1, 3, 10, 10, 25, '../../../assets/wand.png');";

const char insert_spell_sql[] =
        "INSERT INTO SPELL (type, name, amount, cost) VALUES (1, 'Heal', 10, 10),"
        "(2, 'Defense', 10, 10),"
        "(3, 'Attack', 10, 10);";

const char insert_monster_sql[] =
        "INSERT INTO MONSTER (type, name, hp_max, hp, attack, defense, image)"
        "VALUES (0, 'Bat', 10, 10, 2, 1, '../../../assets/bat.png'),"
        "(0, 'Gobelin', 15, 15, 3, 2, '../../../assets/gobelin.png'),"
        "(0, 'Zombie', 20, 20, 4, 3, '../../../assets/zombie.png'),"
        "(0, 'Skeleton', 25, 25, 5, 4, '../../../assets/skeleton.png'),"
        "(0, 'Troll', 30, 30, 6, 5, '../../../assets/troll.png'),"
        "(0, 'Ghost', 35, 35, 7, 6, '../../../assets/ghost.png'),"
        "(0, 'Ghoul', 40, 40, 8, 7, '../../../assets/ghoul.png'),"
        "(0, 'Demon', 45, 45, 9, 8, '../../../assets/demon.png'),"
        "(0, 'Flowey', 50, 50, 10, 9, '../../../assets/flowey.png'),"
        "(2, 'M. Sananes', 100, 100, 10, 10, '../../../assets/sananes.png'),"
        "(2, 'M. Delon', 100, 100, 10, 10, '../../../assets/delon.png'),"
        "(2, 'M. Demacon', 100, 100, 10, 10, '../../../assets/demacon.png'),"
        "(2, 'M. Machavoine', 100, 100, 10, 10, '../../../assets/machavoine.png');";

Table_Info all_tables_info[] = {
        {
                "SELECT name FROM sqlite_master WHERE type='table' AND name='ARMOR';",
                create_armor_table_sql,
                insert_armor_sql
        },
        {
                "SELECT name FROM sqlite_master WHERE type='table' AND name='WEAPON';",
                create_weapon_table_sql,
                insert_weapon_sql
        },
        {
                "SELECT name FROM sqlite_master WHERE type='table' AND name='MONSTER';",
                create_monster_table_sql,
                insert_monster_sql
        },
        {
                "SELECT name FROM sqlite_master WHERE type='table' AND name='INVENTORY';",
                create_inventory_table_sql,
                NULL
        },
        {
                "SELECT name FROM sqlite_master WHERE type='table' AND name='ARMORS_IN_INVENTORY';",
                create_armors_in_inventory_table_sql,
                NULL
        },
        {
                "SELECT name FROM sqlite_master WHERE type='table' AND name='WEAPONS_IN_INVENTORY';",
                create_weapons_in_inventory_table_sql,
                NULL
        },
        {
                "SELECT name FROM sqlite_master WHERE type='table' AND name='SPELL';",
                create_spell_table_sql,
                insert_spell_sql
        },
        {
                "SELECT name FROM sqlite_master WHERE type='table' AND name='PLAYER';",
                create_player_table_sql,
                NULL
        },
        {
                "SELECT name FROM sqlite_master WHERE type='table' AND name='STAGE';",
                create_stage_table_sql,
                NULL
        },
        {
                "SELECT name FROM sqlite_master WHERE type='table' AND name='STATS';",
                create_stats_table_sql,
                NULL
        }
};