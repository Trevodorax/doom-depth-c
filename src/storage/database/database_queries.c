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
        "health_healed INTEGER NOT NULL,"
        "max_level_reached INTEGER NOT NULL"
        ");";

const char create_player_table_sql[] =
        "CREATE TABLE IF NOT EXISTS PLAYER ("
        "    id                 INTEGER primary key autoincrement,"
        "    name               TEXT    not null,"
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
        "    nb_health_potions  INTEGER not null"
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
        "    image    TEXT    not null"
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
        "    image            TEXT    not null"
        ");";

const char create_armors_in_inventory_table_sql[] =
        "CREATE TABLE IF NOT EXISTS ARMORS_IN_INVENTORY ("
        "    player_id INTEGER not null references PLAYER,"
        "    armor_id     INTEGER not null references ARMOR,"
        "    uses         INTEGER not null,"
        "    chosen       INTEGER not null,"
        "    primary key (player_id, armor_id)"
        ");";

const char create_weapons_in_inventory_table_sql[] =
        "CREATE TABLE IF NOT EXISTS WEAPONS_IN_INVENTORY ("
        "    player_id INTEGER not null references PLAYER,"
        "    weapon_id     INTEGER not null references WEAPON,"
        "    uses         INTEGER not null,"
        "    chosen       INTEGER not null,"
        "    primary key (player_id, weapon_id)"
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

const char create_stage_table_sql[] =
        "CREATE TABLE IF NOT EXISTS STAGE ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nb_enemies INTEGER NOT NULL,"
        "enemy_chances_to_appear TEXT NOT NULL"
        ");";

const char insert_armor_sql[] =
        "INSERT INTO ARMOR (name, amount, rarity, max_uses, cost, image)"
        "VALUES ('Leather Armor', 1, 1, 10, 10, '../../../assets/leather_armor.png'),"
        "       ('Chainmail', 1, 1, 20, 20, '../../../assets/chainmail.png'),"
        "       ('Plate Armor', 1, 2, 30, 30, '../../../assets/plate_armor.png'),"
        "       ('Dragon Armor', 1, 2, 40, 40, '../../../assets/dragon_armor.png'),"
        "       ('God Armor', 1, 3, 50, 50, '../../../assets/god_armor.png');";

const char insert_weapon_sql[] =
        "INSERT INTO WEAPON (name, min_attack, max_attack, attacks_per_turn, rarity, max_uses, cost, image)"
        "VALUES ('Butter Knife', 1, 2, 1, 1, 10, 3, '../../../assets/butter_knife.png'),"
        "       ('Compass', 2, 4, 1, 1, 10, 5, '../../../assets/compass.png'),"
        "       ('Wooden Sword', 3, 6, 1, 2, 10, 10, '../../../assets/wooden_sword.png'),"
        "       ('Lightsaber', 4, 8, 1, 2, 10, 15, '../../../assets/lightsaber.png'),"
        "       ('Flip-Flops', 5, 10, 1, 3, 10, 20, '../../../assets/flip_flops.png'),"
        "       ('Wand', 6, 12, 1, 3, 10, 25, '../../../assets/wand.png');";

const char insert_spell_sql[] =
        "INSERT INTO SPELL (type, name, amount, cost) VALUES (1, 'Heal', 10, 10),"
        "(2, 'Defense', 10, 10),"
        "(3, 'Attack', 10, 10);";

const char insert_monster_sql[] =
        "INSERT INTO MONSTER (type, name, hp_max, hp, attack, defense, image)"
        "VALUES (0, 'bat', 10, 10, 2, 1, '../../../assets/bat.png'),"
        "(0, 'goblin', 15, 15, 3, 2, '../../../assets/gobelin.png'),"
        "(0, 'zombie', 20, 20, 4, 3, '../../../assets/zombie.png'),"
        "(0, 'skeleton', 25, 25, 5, 4, '../../../assets/skeleton.png'),"
        "(0, 'troll', 30, 30, 6, 5, '../../../assets/troll.png'),"
        "(0, 'ghost', 35, 35, 7, 6, '../../../assets/ghost.png'),"
        "(0, 'ghoul', 40, 40, 8, 7, '../../../assets/ghoul.png'),"
        "(0, 'demon', 45, 45, 9, 8, '../../../assets/demon.png'),"
        "(0, 'flowey', 50, 50, 10, 9, '../../../assets/flowey.png'),"
        "(2, 'sananes', 100, 100, 10, 10, '../../../assets/sananes.png'),"
        "(2, 'delon', 100, 100, 10, 10, '../../../assets/delon.png'),"
        "(2, 'demacon', 100, 100, 10, 10, '../../../assets/demacon.png'),"
        "(2, 'machavoine', 100, 100, 10, 10, '../../../assets/machavoine.png');";

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
        "nb_health_potions = ? "
        "WHERE id = ?;";