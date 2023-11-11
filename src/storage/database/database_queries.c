#include "database_queries.h"


const char create_spell_table_sql[] =
        "CREATE TABLE IF NOT EXISTS SPELL ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "type INTEGER NOT NULL,"
        "name TEXT NOT NULL,"
        "amount INTEGER NOT NULL,"
        "cost INTEGER NOT NULL"
        ");";

const char create_stats_table_sql[] =
        "CREATE TABLE IF NOT EXISTS STATS ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nb_monsters_killed INTEGER NOT NULL,"
        "nb_deaths INTEGER NOT NULL,"
        "damages_dealt INTEGER NOT NULL,"
        "health_healed INTEGER NOT NULL"
        ");";

const char create_player_table_sql[] =
        "CREATE TABLE IF NOT EXISTS PLAYER ("
        "    id                 INTEGER primary key autoincrement,"
        "    name               TEXT    not null,"
        "    current_map        TEXT    not null,"
        "    hp                 INTEGER not null,"
        "    hp_max             INTEGER not null,"
        "    mana               INTEGER not null,"
        "    mana_max           INTEGER not null,"
        "    gold               INTEGER not null,"
        "    xp                 INTEGER not null,"
        "    level              INTEGER not null,"
        "    base_attack        INTEGER not null,"
        "    base_defense       INTEGER not null,"
        "    offensive_spell_id INTEGER references SPELL,"
        "    defensive_spell_id INTEGER references SPELL,"
        "    healing_spell_id   INTEGER references SPELL,"
        "    stats_id           INTEGER not null references STATS,"
        "    inventory_capacity INTEGER not null,"
        "    nb_weapons         INTEGER not null,"
        "    nb_armors          INTEGER not null,"
        "    nb_mana_potions    INTEGER not null,"
        "    nb_health_potions  INTEGER not null,"
        "    action_points      INTEGER not null,"
        "    max_action_points  INTEGER not null"
        ");";

const char create_armor_table_sql[] =
        "CREATE TABLE IF NOT EXISTS ARMOR ("
        "id       INTEGER"
        "        primary key autoincrement,"
        "    name     TEXT    not null,"
        "    amount   INTEGER not null,"
        "    rarity   INTEGER not null,"
        "    max_uses INTEGER not null,"
        "    cost     INTEGER not null,"
        "    image    TEXT    not null,"
        "    ascii_path TEXT not null"
        ");";

const char create_weapon_table_sql[] =
        "CREATE TABLE IF NOT EXISTS WEAPON ("
        "    id               INTEGER"
        "        primary key autoincrement,"
        "    name             TEXT    not null,"
        "    min_attack       INTEGER not null,"
        "    max_attack       INTEGER not null,"
        "    attacks_per_turn INTEGER not null,"
        "    rarity           INTEGER not null,"
        "    max_uses         INTEGER not null,"
        "    cost             INTEGER not null,"
        "    image            TEXT    not null,"
        "    ascii_path       TEXT    not null"
        ");";

const char create_armors_in_inventory_table_sql[] =
        "CREATE TABLE IF NOT EXISTS ARMORS_IN_INVENTORY ("
        "    id INTEGER primary key autoincrement,"
        "    player_id INTEGER not null references PLAYER,"
        "    armor_id     INTEGER not null references ARMOR,"
        "    uses         INTEGER not null,"
        "    chosen       INTEGER not null"
        ");";

const char create_weapons_in_inventory_table_sql[] =
        "CREATE TABLE IF NOT EXISTS WEAPONS_IN_INVENTORY ("
        "    id INTEGER primary key autoincrement,"
        "    player_id INTEGER not null references PLAYER,"
        "    weapon_id     INTEGER not null references WEAPON,"
        "    uses         INTEGER not null,"
        "    chosen       INTEGER not null"
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
        "image TEXT NOT NULL,"
        "asciiart TEXT NOT NULL"
        ");";

const char create_stage_table_sql[] =
        "CREATE TABLE IF NOT EXISTS STAGE ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nb_enemies INTEGER NOT NULL,"
        "enemy_chances_to_appear TEXT NOT NULL"
        ");";

const char insert_armor_sql[] =
        "INSERT INTO ARMOR (name, amount, rarity, max_uses, cost, image, ascii_path)"
        "VALUES ('Leather Armor', 1, 1, 10, 10, '../assets/armors/image/leather.png', '../assets/armors/ascii/leather.asciiart'),"
        "       ('Chainmail', 2, 1, 20, 20, '../assets/armors/image/chain_mail.png', '../assets/armors/ascii/chain_mail.asciiart'),"
        "       ('Plate Armor', 3, 2, 30, 30, '../assets/armors/image/plate.png', '../assets/armors/ascii/plate.asciiart'),"
        "       ('Dragon Armor', 5, 2, 40, 40, '../assets/armors/image/dragon.png', '../assets/armors/ascii/dragon.asciiart'),"
        "       ('God Armor', 7, 5, 50, 50, '../assets/armors/image/god.png', '../assets/armors/ascii/god.asciiart');";

const char insert_weapon_sql[] =
        "INSERT INTO WEAPON (name, min_attack, max_attack, attacks_per_turn, rarity, max_uses, cost, image, ascii_path)"
        "VALUES ('Butter Knife', 1, 2, 1, 1, 10, 3, '../assets/weapons/image/butter_knife.png', '../assets/weapons/ascii/butter_knife.asciiart'),"
        "       ('Compass', 2, 4, 1, 1, 10, 5, '../assets/weapons/image/compass.png', '../assets/weapons/ascii/compass.asciiart'),"
        "       ('Wooden Sword', 3, 6, 1, 2, 10, 10, '../assets/weapons/image/wooden_sword.png', '../assets/weapons/ascii/wooden_sword.asciiart'),"
        "       ('Lightsaber', 4, 8, 1, 2, 10, 15, '../assets/weapons/image/lightsaber.png', '../assets/weapons/ascii/lightsaber.asciiart'),"
        "       ('Flip-Flops', 5, 10, 1, 3, 10, 20, '../assets/weapons/image/flip_flop.png', '../assets/weapons/ascii/flip_flop.asciiart'),"
        "       ('Wand', 6, 12, 1, 3, 10, 25, '../assets/weapons/image/elder_wand.png', '../assets/weapons/ascii/elder_wand.asciiart');";

const char insert_spell_sql[] =
        "INSERT INTO SPELL (type, name, amount, cost) VALUES (1, 'Heal', 10, 10),"
        "(2, 'Defense', 10, 10),"
        "(3, 'Attack', 10, 10);";

const char insert_monster_sql[] =
        "INSERT INTO MONSTER (type, name, hp_max, hp, attack, defense, image, asciiart)"
        "VALUES (0, 'bat', 10, 10, 2, 1, '../assets/monsters/image/bat.png', '../assets/monsters/ascii/bat.asciiart'),"
        "(0, 'goblin', 15, 15, 3, 2, '../assets/monsters/image/goblin.png', '../assets/monsters/ascii/goblin.asciiart'),"
        "(0, 'zombie', 20, 20, 4, 3, '../assets/monsters/image/zombie.png', '../assets/monsters/ascii/zombie.asciiart'),"
        "(0, 'skeleton', 25, 25, 5, 4, '../assets/monsters/image/skeleton.png', '../assets/monsters/ascii/skeleton.asciiart'),"
        "(0, 'troll', 30, 30, 6, 5, '../assets/monsters/image/troll.png', '../assets/monsters/ascii/troll.asciiart'),"
        "(0, 'ghost', 35, 35, 7, 6, '../assets/monsters/image/ghost.png', '../assets/monsters/ascii/ghost.asciiart'),"
        "(0, 'ghoul', 40, 40, 8, 7, '../assets/monsters/image/ghoul.png', '../assets/monsters/ascii/ghoul.asciiart'),"
        "(0, 'demon', 45, 45, 9, 8, '../assets/monsters/image/demon.png', '../assets/monsters/ascii/demon.asciiart'),"
        "(0, 'flowey', 50, 50, 10, 9, '../assets/monsters/image/flowey.png', '../assets/monsters/ascii/flowey.asciiart'),"
        "(2, 'sananes', 100, 100, 10, 10, '../assets/monsters/image/sananes.png', '../assets/monsters/ascii/sananes.asciiart'),"
        "(2, 'delon', 100, 100, 10, 10, '../assets/monsters/image/delon.png', '../assets/monsters/ascii/delon.asciiart'),"
        "(2, 'demacon', 100, 100, 10, 10, '../assets/monsters/image/demacon.png', '../assets/monsters/ascii/demacon.asciiart'),"
        "(2, 'machavoine', 100, 100, 10, 10, '../assets/monsters/image/machavoine.png', '../assets/monsters/ascii/machavoine.asciiart');";

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

const char update_player_sql[] =
        "UPDATE PLAYER SET "
        "hp = ?, "
        "hp_max = ?, "
        "mana = ?, "
        "mana_max = ?, "
        "xp = ?, "
        "level = ?, "
        "base_attack = ?, "
        "base_defense = ?, "
        "gold = ?, "
        "offensive_spell_id = ?, "
        "defensive_spell_id = ?, "
        "healing_spell_id = ?, "
        "inventory_capacity = ?, "
        "nb_weapons = ?, "
        "nb_armors = ?, "
        "nb_mana_potions = ?, "
        "nb_health_potions = ?, "
        "action_points = ?, "
        "max_action_points = ?, "
        "current_map = ? "
        "WHERE id = ?;";

const char create_new_player_sql[] =
        "INSERT INTO PLAYER (name, current_map, hp, hp_max, mana, mana_max, xp, level, base_attack, base_defense, gold, offensive_spell_id, defensive_spell_id, healing_spell_id, stats_id, inventory_capacity, nb_weapons, nb_armors, nb_mana_potions, nb_health_potions, action_points, max_action_points) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

const char create_player_from_db_sql[] =
        "SELECT id, name, hp, hp_max, mana, mana_max, gold, xp, level, base_attack, "
        "base_defense, action_points, max_action_points, offensive_spell_id, defensive_spell_id, healing_spell_id, "
        "current_map FROM PLAYER WHERE id = %d;";

const char delete_player_from_db_sql[] =
        "DELETE FROM PLAYER WHERE id = %d;";

const char delete_stats_from_db_sql[] =
        "DELETE FROM STATS WHERE id = %d;";

const char delete_weapons_in_inventory_from_db_sql[] =
        "DELETE FROM WEAPONS_IN_INVENTORY WHERE player_id = %d;";

const char delete_armors_in_inventory_from_db_sql[] =
        "DELETE FROM ARMORS_IN_INVENTORY WHERE player_id = %d;";