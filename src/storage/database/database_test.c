#include <stdio.h>
#include "../../minunit/minunit.h"
#include "database.h"

char * test_db_connection() {
    sqlite3 *db = db_connection();
    mu_assert("Error in test_db_connection: db is null", db != NULL);
    return 0;
}

char * all_tests() {
    mu_run_test(test_db_connection);
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