#include <stdio.h>
#include "../minunit/minunit.h"
#include "map_test.h"
#include "map.h"

static char * test_json_to_map() {
    char *json_str =
            "{"
            "\"name\":\"TestMap\","
            "\"stages\":[{"
            "\"nb_monsters\":0,"
            "\"done\":1,"
            "\"start\":1"
            "}]"
            "}";

    Json *json_obj = parse_json(&json_str);

    map_t *map = json_to_map(json_obj);
    mu_assert("error, map is NULL", map != NULL);
    mu_assert("error, map name is not TestMap", strcmp(map->name, "TestMap") == 0);

    mu_assert("error, first_stage is NULL", map->first_stage != NULL);
    mu_assert("error, first_stage type is not EMPTY", map->first_stage->type == EMPTY);
    mu_assert("error, first_stage is_done flag is not true", map->first_stage->is_done == true);

    map_t *null_map = json_to_map(NULL);
    mu_assert("error, null_map is not NULL", null_map == NULL);

    Json invalid_json_obj;
    invalid_json_obj.type = 'n';
    invalid_json_obj.number = 42;

    map_t *invalid_map = json_to_map(&invalid_json_obj);
    mu_assert("error, invalid_map is not NULL", invalid_map == NULL);

    // test missing fields
    char *json_str_missing_fields =
            "{"
            "\"name\":\"TestMap\""
            "}";
    Json *missing_fields_json_obj = parse_json(&json_str_missing_fields);

    mu_assert("error, missing_fields_json_obj is NULL", missing_fields_json_obj != NULL); // Asserting that parsing didn't fail outright
    map_t *missing_fields_map = json_to_map(missing_fields_json_obj);
    mu_assert("error, missing_fields_map is not NULL", missing_fields_map == NULL);

    free_json(missing_fields_json_obj);
    free_json(json_obj);

    return 0;
}

static char * all_tests() {
    mu_run_test(test_json_to_map);
    return 0;
}

int map_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED IN MAP\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}