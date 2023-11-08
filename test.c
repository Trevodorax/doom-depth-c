#include "src/utils/array_test.h"
#include "src/storage/json/json_test.h"
#include "src/storage/database/database_test.h"
#include "src/map_screen/map/map_test.h"
#include "src/ui_utils/cli_utils/ascii_art/ascii_art_test.h"

int tests_run = 0;

int main(int argc, char **argv) {
    int ascii = ascii_test();
    int json = json_test();
    int array = array_test();
    int database = database_test();
    int map = map_test();

    return ascii && json && array && database && map;
}
