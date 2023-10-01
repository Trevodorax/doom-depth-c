#include "array.h"
#include "array_test.h"
#include "../minunit/minunit.h"
#include <stdio.h>
#include <string.h>

char * test_get_node_int_value_at_index() {
    array_node_t *start = NULL;

    unsigned int_size = sizeof(int);
    int arr[] = {10, 20, 30, 40, 50}, i;
    for (i = 4; i >= 0; i--) {
        push(&start, &arr[i], int_size);
    }
    int * assertValue = (int *)get_value_at_index(start,1);
    mu_assert("\nError in test_get_node_at_index : WRONG VALUE - Expecting 20", *assertValue == 20);
    return 0;
}

char * test_get_node_float_value_at_index() {
    array_node_t *start = NULL;

    // Example with floats
    unsigned float_size = sizeof(float);
    start = NULL;
    float arr_f[] = {10.1f, 20.2f, 30.3f, 40.4f, 50.5f};
    for (int i = 4; i >= 0; i--) {
        push(&start, &arr_f[i], float_size);
    }
    float * assertValue = (float *)get_value_at_index(start,1);
    mu_assert("Error in test float array", *assertValue == 20.2f);
    return 0;
}

char * test_get_node_string_value_at_index() {
    array_node_t *start = NULL;

    // Example with strings
    unsigned string_size = sizeof(char *);
    char *arr_s[] = {"one", "two", "three", "four", "five"};
    for (int i = 4; i >= 0; i--) {
        push(&start, &arr_s[i], string_size);
    }
    char **assertValue = (char **)get_value_at_index(start, 1);
    mu_assert("Error in test string array", strcmp(*assertValue, "two") == 0);
    return 0;
}

char * all_array_tests() {
    mu_run_test(test_get_node_int_value_at_index);
    mu_run_test(test_get_node_float_value_at_index);
    mu_run_test(test_get_node_string_value_at_index);
    return 0;
}

int array_test() {
    char *result = all_array_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED IN ARRAY\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}

