#include <stdio.h>
#include "../../minunit/minunit.h"
#include "database.h"
#include "../../entities/monster/monster.h"
#include "../../entities/armor/armor.h"
#include "../../entities/weapon/weapon.h"

char * test_db_connection() {
    sqlite3 *db = db_connection();
    mu_assert("Error in test_db_connection: db is null", db != NULL);
    return 0;
}

char * test_save_player() {
    sqlite3 *db = db_connection();
    player_t *player = create_player("test");
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
    array_node_t *stats = create_struct_from_db(db, "SELECT * FROM STATS", create_stats_from_db, sizeof (stats_t));
    print_list(stats, print_stats);
    mu_assert("Error in test_create_stats_from_db: stats is null", stats != NULL);
    return 0;
}

char * all_tests() {
    mu_run_test(test_db_connection);
    mu_run_test(test_save_player);
    // mu_run_test(test_create_monsters_from_db);
    // mu_run_test(test_create_armors_from_db);
    // mu_run_test(test_create_weapons_from_db);
    // mu_run_test(test_create_spells_from_db);
    mu_run_test(test_create_stats_from_db);
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