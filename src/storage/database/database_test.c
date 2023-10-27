#include <stdio.h>
#include "../../minunit/minunit.h"
#include "database.h"
#include "../../entities/monster/monster.h"
#include "../../entities/armor/armor.h"
#include "../../entities/weapon/weapon.h"
#include "../../entities/spell/spell.h"
#include "../../entities/stats/stats.h"
#include "../../entities/inventory/inventory.h"
#include "../../entities/player/player.h"

char * test_db_connection() {
    sqlite3 *db = db_connection();
    mu_assert("Error in test_db_connection: db is null", db != NULL);
    return 0;
}

void print_player(void *player) {
    player_t *p = (player_t *)player;
    printf("id: %d\n", p->id);
    printf("name: %s\n", p->name);
    printf("hp: %d\n", p->hp);
    printf("hp_max: %d\n", p->hp_max);
    printf("mana: %d\n", p->mana);
    printf("mana_max: %d\n", p->mana_max);
    printf("gold: %d\n", p->gold);
    printf("xp: %d\n", p->xp);
    printf("level: %d\n", p->level);
    printf("base_attack: %d\n", p->base_attack);
    printf("base_defense: %d\n", p->base_defense);
    printf("offensive_spell: %s\n", p->offensive_spell->name);
    printf("defensive_spell: %s\n", p->defensive_spell->name);
    printf("healing_spell: %s\n", p->healing_spell->name);
    if (p->chosen_armor != NULL) {
        printf("chosen_armor: %s\n", p->chosen_armor->name);
    }
    else {
        printf("chosen_armor: NULL\n");
    }
    if (p->chosen_weapon != NULL) {
        printf("chosen_weapon: %s\n", p->chosen_weapon->name);
    }
    else {
        printf("chosen_weapon: NULL\n");
    }
    printf("inventory: %d\n", p->inventory->nb_mana_potions);
    printf("stats: %d\n", p->stats->damages_dealt);
}

char * test_save_player() {
    sqlite3 *db = db_connection();

    // get the player in DB
    char *sql = "SELECT * FROM PLAYER WHERE id = 4";
    array_node_t *players = create_struct_from_db(db, sql, create_player_from_db, sizeof (player_t));
    player_t *player = (player_t *)players->value;
    player->action_points = 2;

    print_player(player);

    int rc = save_player(db, player);
    mu_assert("Error in test_save_player: rc is not SQLITE_OK", rc == SQLITE_OK);
    return 0;
}

void print_monster(void *monster) {
    monster_t *m = (monster_t *)monster;
    printf("id: %d\n", m->id);
    printf("type: %d\n", m->type);
    printf("name: %s\n", m->name);
    printf("hp: %d\n", m->hp);
    printf("hp_max: %d\n", m->hp_max);
    printf("attack: %d\n", m->attack);
    printf("defense: %d\n", m->defense);
    printf("image: %s\n", m->image_path);
}

char * test_create_monsters_from_db() {
    sqlite3 *db = db_connection();
    array_node_t *monsters = create_struct_from_db(db, "SELECT * FROM MONSTER", create_monster_from_db, sizeof (monster_t));
    print_list(monsters, print_monster);
    mu_assert("Error in test_create_monsters_from_db: monsters is null", monsters != NULL);
    return 0;
}

void print_armors(void *armor) {
    armor_t *a = (armor_t *)armor;
    printf("id: %d\n", a->id);
    printf("name: %s\n", a->name);
    printf("rarity: %d\n", a->rarity);
    printf("amount: %d\n", a->amount);
    printf("max_uses: %d\n", a->max_uses);
    printf("uses: %d\n", a->uses);
    printf("cost: %d\n", a->cost);
    printf("image: %s\n", a->image_path);
}

char * test_create_armors_from_db() {
    sqlite3 *db = db_connection();
    array_node_t *armors = create_struct_from_db(db, "SELECT * FROM ARMOR", create_armor_from_db, sizeof (armor_t));
    print_list(armors, print_armors);
    mu_assert("Error in test_create_armors_from_db: armors is null", armors != NULL);
    return 0;
}

void print_weapons(void *weapon) {
    weapon_t *w = (weapon_t *)weapon;
    printf("id: %d\n", w->id);
    printf("name: %s\n", w->name);
    printf("min_attack: %d\n", w->min_attack);
    printf("max_attack: %d\n", w->max_attack);
    printf("attacks_per_turn: %d\n", w->attacks_per_turn);
    printf("rarity: %d\n", w->rarity);
    printf("max_uses: %d\n", w->max_uses);
    printf("uses: %d\n", w->uses);
    printf("cost: %d\n", w->cost);
    printf("image: %s\n", w->image_path);
}

char * test_create_weapons_from_db() {
    sqlite3 *db = db_connection();
    array_node_t *weapons = create_struct_from_db(db, "SELECT * FROM WEAPON", create_weapon_from_db, sizeof (weapon_t));
    print_list(weapons, print_weapons);
    mu_assert("Error in test_create_weapons_from_db: weapons is null", weapons != NULL);
    return 0;
}

void print_spells(void *spell) {
    spell_t *s = (spell_t *)spell;
    printf("id: %d\n", s->id);
    printf("type: %d\n", s->type);
    printf("name: %s\n", s->name);
    printf("amount: %d\n", s->amount);
    printf("cost: %d\n", s->cost);
}

char * test_create_spells_from_db() {
    sqlite3 *db = db_connection();
    array_node_t *spells = create_struct_from_db(db, "SELECT * FROM SPELL", create_spell_from_db, sizeof (spell_t));
    print_list(spells, print_spells);
    mu_assert("Error in test_create_spells_from_db: spells is null", spells != NULL);
    return 0;
}

void print_stats(void *stats) {
    stats_t *s = (stats_t *)stats;
    printf("id: %d\n", s->id);
    printf("nb_monsters_killed: %d\n", s->nb_monsters_killed);
    printf("nb_deaths: %d\n", s->nb_deaths);
    printf("damages_dealt: %d\n", s->damages_dealt);
    printf("health_healed: %d\n", s->health_healed);
    printf("max_level_reached: %d\n", s->max_level_reached);
}

char * test_create_stats_from_db() {
    sqlite3 *db = db_connection();

    // in the game, the player_id is the id of the player who is playing
    int player_id = 1;

    char *sql = malloc(sizeof(char) * 100);
    sprintf(sql, "SELECT * FROM STATS WHERE id = (SELECT stats_id FROM PLAYER WHERE id = %d)", player_id);

    array_node_t *stats = create_struct_from_db(db, sql, create_stats_from_db, sizeof (stats_t));
    print_list(stats, print_stats);

    free(sql);
    mu_assert("Error in test_create_stats_from_db: stats is null", stats != NULL);
    return 0;
}

void print_inventory(void *inventory) {
    inventory_t *i = (inventory_t *)inventory;
    printf("capacity: %d\n", i->capacity);
    printf("nb_weapons: %d\n", i->nb_weapons);
    printf("nb_armors: %d\n", i->nb_armors);
    printf("nb_mana_potions: %d\n", i->nb_mana_potions);
    printf("nb_health_potions: %d\n", i->nb_health_potions);
}

char * test_create_inventory_from_db() {
    sqlite3 *db = db_connection();

    // in the game, the player_id is the id of the player who is playing
    int player_id = 1;

    array_node_t *inventory = create_full_inventory_from_db(db, player_id);

    print_list(inventory, print_inventory);
    mu_assert("Error in test_create_inventory_from_db: inventory is null", inventory != NULL);
    return 0;
}

char *test_get_players_from_db() {
    sqlite3 *db = db_connection();
    array_node_t *players = players_from_db(db);
    array_node_t *head = players;

    while (players != NULL) {
        player_t *player = (player_t *)players->value;
        printf("id: %d\n", player->id);
        printf("name: %s\n", player->name);
        players = players->next;
    }

    mu_assert("Error in test_get_players_from_db: players is null", head != NULL);
    return 0;
}

char * all_tests() {
    // mu_run_test(test_db_connection);
    // mu_run_test(test_save_player);
    // mu_run_test(test_create_monsters_from_db);
    // mu_run_test(test_create_armors_from_db);
    // mu_run_test(test_create_weapons_from_db);
    // mu_run_test(test_create_spells_from_db);
    // mu_run_test(test_create_stats_from_db);
    // mu_run_test(test_create_inventory_from_db);
    mu_run_test(test_get_players_from_db);
    return 0;
}

int database_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED IN DATABASE\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}