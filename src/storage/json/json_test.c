#include <stdio.h>
#include <string.h>
#include "json.h"
#include "json_test.h"
#include "../../minunit/minunit.h"

static char * test_parse_simple_json_string() {
    char *json_str = "{\"name\":\"Paul\", \"age\":20, \"city\":\"Paris\"}";
    Json *json_obj = parse_json(&json_str);

    mu_assert("parse_json error, the json_obj should be valid ", json_obj != NULL);
    mu_assert("parse_json error, json_obj->type != 'o'", json_obj->type == 'o');
    mu_assert("parse_json error, json_obj->nb_elements != 3", json_obj->nb_elements == 3);
    mu_assert("parse_json error, key[0] != name", strcmp(json_obj->keys[0], "name") == 0);
    mu_assert("parse_json error, value[0] != Paul", strcmp(json_obj->values[0].string, "Paul") == 0);
    mu_assert("parse_json error, key[1] != age", strcmp(json_obj->keys[1], "age") == 0);
    mu_assert("parse_json error, value[1] != 20", json_obj->values[1].number == 20);
    mu_assert("parse_json error, key[2] != city", strcmp(json_obj->keys[2], "city") == 0);
    mu_assert("parse_json error, value[2] != Paris", strcmp(json_obj->values[2].string, "Paris") == 0);

    free_json(json_obj);

    return 0;
}

static char * test_parse_complex_json_string() {
    char *json_str =
            "{"
            "\"name\":\"Paul\","
            "\"age\":20,"
            "\"city\":\"Paris\","
            "\"friends\":[\"Anaelle\",\"Noam\", \"Tom\"],"
            "\"address\":{"
            "\"street\":\"1 rue du Pain\","
            "\"zipcode\":\"12345\""
            "}"
            "}";

    Json *json_obj = parse_json(&json_str);

    mu_assert("parse_json error, the json_obj should be valid", json_obj != NULL);
    mu_assert("parse_json error, json_obj->type != 'o'", json_obj->type == 'o');
    mu_assert("parse_json error, json_obj->nb_elements != 5", json_obj->nb_elements == 5);

    // simple fields
    mu_assert("parse_json error, key[0] != name", strcmp(json_obj->keys[0], "name") == 0);
    mu_assert("parse_json error, value[0] != Paul", strcmp(json_obj->values[0].string, "Paul") == 0);
    mu_assert("parse_json error, key[1] != age", strcmp(json_obj->keys[1], "age") == 0);
    mu_assert("parse_json error, value[1] != 20", json_obj->values[1].number == 20);
    mu_assert("parse_json error, key[2] != city", strcmp(json_obj->keys[2], "city") == 0);
    mu_assert("parse_json error, value[2] != Paris", strcmp(json_obj->values[2].string, "Paris") == 0);

    // array
    mu_assert("parse_json error, key[3] != friends", strcmp(json_obj->keys[3], "friends") == 0);
    mu_assert("parse_json error, value[3].type != 'a'", json_obj->values[3].type == 'a');
    mu_assert("parse_json error, value[3].nb_elements != 3", json_obj->values[3].nb_elements == 3);
    mu_assert("parse_json error, value[3].values[0] != Anaelle", strcmp(json_obj->values[3].values[0].string, "Anaelle") == 0);
    mu_assert("parse_json error, value[3].values[1] != Noam", strcmp(json_obj->values[3].values[1].string, "Noam") == 0);
    mu_assert("parse_json error, value[3].values[2] != Tom", strcmp(json_obj->values[3].values[2].string, "Tom") == 0);

    // nested
    mu_assert("parse_json error, key[4] != address", strcmp(json_obj->keys[4], "address") == 0);
    mu_assert("parse_json error, value[4].type != 'o'", json_obj->values[4].type == 'o');
    mu_assert("parse_json error, value[4].nb_elements != 2", json_obj->values[4].nb_elements == 2);
    mu_assert("parse_json error, value[4].keys[0] != street", strcmp(json_obj->values[4].keys[0], "street") == 0);
    mu_assert("parse_json error, value[4].values[0] != 1 rue du Pain", strcmp(json_obj->values[4].values[0].string, "1 rue du Pain") == 0);
    mu_assert("parse_json error, value[4].keys[1] != zipcode", strcmp(json_obj->values[4].keys[1], "zipcode") == 0);
    mu_assert("parse_json error, value[4].values[1] != 12345", strcmp(json_obj->values[4].values[1].string, "12345") == 0);

    free_json(json_obj);

    return 0;
}


static char * test_parse_invalid_json_string() {
    char *json_str = "{\"name\":\"Paul\", \"age\":20, \"city\":\"Paris\", \"friends\": [\"tom\"A]}"; // Missing comma and incorrect key format
    Json *json_obj = parse_json(&json_str);

    mu_assert("parse_json error, json_obj should be NULL for invalid json", json_obj == NULL);

    return 0;
}

static char * all_tests() {
    mu_run_test(test_parse_simple_json_string);
    mu_run_test(test_parse_complex_json_string);
    mu_run_test(test_parse_invalid_json_string);
    return 0;
}


int json_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED IN JSON\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
