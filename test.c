#include "src/utils/array_test.h"
#include "src/storage/json/json_test.h"
#include "src/storage/database/database_test.h"

int tests_run = 0;

int main(int argc, char **argv) {
    int json = json_test();
    int array = array_test();
    int database = database_test();
    return json && array;
}
