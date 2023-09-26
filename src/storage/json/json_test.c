#include <stdio.h>
#include "../../minunit/minunit.h"

int foo = 7;
int bar = 4;

int getRectanglePerimeter(int h, int w) {
    return 2 * h + 2 * w;
}

char * test_perimeter_1() {
    mu_assert("Error in getRectanglePerimeter: wrong perimeter", getRectanglePerimeter(2, 3) == 10);
    return 0;
}

char * test_perimeter_2() {
    mu_assert("Error in getRectanglePerimeter: wrong perimeter", getRectanglePerimeter(3, 3) == 12);
    return 0;
}

char * all_json_tests() {
    mu_run_test(test_perimeter_1);
    mu_run_test(test_perimeter_2);
    return 0;
}

int json_test() {
    char *result = all_json_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED IN JSON\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
