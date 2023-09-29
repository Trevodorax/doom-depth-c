#include <stdio.h>
#include "../minunit/minunit.h"
#include "map_test.h"
#include "map.h"

map_t * json_to_map(Json * json_map);
int get_map_dimensions(map_t *map, int *width, int *height);

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

static char * test_get_map_dimensions() {
    char *string_map = "{"
                      "\"name\": \"Test Map\","
                      "\"stages\": ["
                          "{"
                              "\"nb_monsters\": 0,"
                              "\"done\": 1,"
                              "\"start\": 1,"
                               "\"left\": {"
                                   "\"nb_monsters\": 3,"
                                   "\"enemy_chances_to_appear\": [50, 50],"
                                   "\"done\": 0"
                               "},"
                              "\"right\": {"
                                  "\"nb_monsters\": 2,"
                                  "\"enemy_chances_to_appear\": [50, 50],"
                                  "\"done\": 0,"
                                  "\"right\": {"
                                      "\"nb_monsters\": 4,"
                                      "\"enemy_chances_to_appear\": [30, 40, 30],"
                                      "\"done\": 0,"
                                      "\"right\": {"
                                          "\"nb_monsters\": 1,"
                                          "\"enemy_chances_to_appear\": [0, 0, 0, 100],"
                                          "\"done\": 0,"
                                          "\"linked_map\": \"map_2.json\""
                                      "},"
                                      "\"top\": {"
                                          "\"nb_monsters\": 3,"
                                          "\"enemy_chances_to_appear\": [50, 50],"
                                          "\"done\": 0,"
                                           "\"top\": {"
                                               "\"nb_monsters\": 3,"
                                               "\"enemy_chances_to_appear\": [50, 50],"
                                               "\"done\": 0"
                                           "}"
                                      "},"
                                      "\"bottom\": {"
                                          "\"nb_monsters\": 3,"
                                          "\"enemy_chances_to_appear\": [50, 50],"
                                          "\"done\": 0,"
                                          "\"bottom\": {"
                                              "\"nb_monsters\": 3,"
                                              "\"enemy_chances_to_appear\": [50, 50],"
                                              "\"done\": 0,"
                                               "\"bottom\": {"
                                                   "\"nb_monsters\": 3,"
                                                   "\"enemy_chances_to_appear\": [50, 50],"
                                                   "\"done\": 0,"
                                                   "\"bottom\": {"
                                                       "\"nb_monsters\": 3,"
                                                       "\"enemy_chances_to_appear\": [50, 50],"
                                                       "\"done\": 0"
                                                   "}"
                                              "}"
                                          "}"
                                      "}"
                                  "}"
                              "}"
                          "}"
                      "]"
                      "}";

    Json *json_map = parse_json(&string_map);

    map_t *map = json_to_map(json_map);
    mu_assert("\ntest_get_map_dimensions error, map is NULL", map != NULL);
    mu_assert("\ntest_get_map_dimensions error, map name is not TestMap", strcmp(map->name, "Test Map") == 0);

    int width;
    int height;

    get_map_dimensions(map, &width, &height);

    printf("\nwidth: %d", width);
    printf("\nheight: %d", height);

    mu_assert("\ntest_get_map_dimensions error wrong width", width == 5);
    mu_assert("\ntest_get_map_dimensions error wrong height", height == 7);
}

static char * all_tests() {
    mu_run_test(test_json_to_map);
    mu_run_test(test_get_map_dimensions);
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