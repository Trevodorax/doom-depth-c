#include <stdio.h>
#include "../../minunit/minunit.h"
#include "database.h"
#include "../../entities/monster/monster.h"
#include "../../entities/armor/armor.h"

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
    array_node_t *monsters = create_struct_from_db(db, "SELECT * FROM MONSTER", create_monster_from_db);
    // print_list(monsters, print_monster);
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
    array_node_t *armors = create_struct_from_db(db, "SELECT * FROM ARMOR", create_armor_from_db);
    print_list(armors, print_armors);
    mu_assert("Error in test_create_armors_from_db: armors is null", armors != NULL);
    return 0;
}

char * all_tests() {
    mu_run_test(test_db_connection);
    mu_run_test(test_save_player);
    // mu_run_test(test_create_monsters_from_db);
    mu_run_test(test_create_armors_from_db);
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