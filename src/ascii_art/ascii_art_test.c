#include "ascii_art_test.h"
#include "../minunit/minunit.h"
#include "../cli_utils/cli_utils.h"
#include "ascii_art.h"

static char * test_ascii_art_to_matrix() {
    char *test_art = ",#####,\n|a` `a|\n|  u  |\n\\ = /";
    cli_matrix_t *result = ascii_art_to_matrix(test_art);

    mu_assert("\nascii_art_to_matrix test failed: wrong number of rows.", result->nb_rows == 4);
    mu_assert("\nascii_art_to_matrix test failed: wrong number of cols.", result->nb_cols == 7);

    mu_assert("\nascii_art_to_matrix test failed: wrong chars", result->matrix[0][0].character == ',');
    mu_assert("\nascii_art_to_matrix test failed: wrong chars", result->matrix[0][0].color == WHITE);

    mu_assert("\nascii_art_to_matrix test failed: wrong chars", result->matrix[1][0].character == '|');
    mu_assert("\nascii_art_to_matrix test failed: wrong chars", result->matrix[1][1].character == 'a');

    return 0;
}

static char * test_parse_ascii_art_file() {
    char *file_path = "../assets/player/ascii/player.asciiart";
    ascii_art_t *result = parse_ascii_art_file(file_path);

    mu_assert("\nparse_ascii_art_file test failed: wrong number of versions.", result->nb_versions == 3);

    mu_assert("\nparse_ascii_art_file test failed: wrong number of rows for version 1.", result->versions[0]->nb_rows == 4);
    mu_assert("\nparse_ascii_art_file test failed: wrong number of cols for version 1.", result->versions[0]->nb_cols == 7);
    mu_assert("\nparse_ascii_art_file test failed: wrong chars for version 1", result->versions[0]->matrix[0][0].character == ',');
    mu_assert("\nparse_ascii_art_file test failed: wrong chars for version 1", result->versions[0]->matrix[2][3].character == 'u');

    mu_assert("\nparse_ascii_art_file test failed: wrong number of rows for version 2.", result->versions[1]->nb_rows == 9);
    mu_assert("\nparse_ascii_art_file test failed: wrong number of cols for version 2.", result->versions[1]->nb_cols == 9);
    mu_assert("\nparse_ascii_art_file test failed: wrong chars for version 2", result->versions[1]->matrix[1][3].character == '|');

    mu_assert("\nparse_ascii_art_file test failed: wrong number of rows for version 3.", result->versions[2]->nb_rows == 20);
    mu_assert("\nparse_ascii_art_file test failed: wrong number of cols for version 3.", result->versions[2]->nb_cols == 18);
    mu_assert("\nparse_ascii_art_file test failed: wrong chars for version 3", result->versions[2]->matrix[6][9].character == ':');

    return 0;
}

static char * all_tests() {
    mu_run_test(test_ascii_art_to_matrix);
    mu_run_test(test_parse_ascii_art_file);
    return 0;
}


int ascii_test() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED IN ASCII\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
