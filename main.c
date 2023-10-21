#include "src/doom_depth.h"
#include "src/cli_utils/cli_utils.h"

#include <stdio.h>

int main(int argc, char * argv[]) {
    /*
    ui_type_t ui_type = get_ui_type(argv[1]);

    doom_depth_main doom_depth = doom_depth_factory(ui_type);

    return doom_depth();
     */

    ascii_art_t * test = get_letter_ascii_art('8');
    for(int i = 0; i < test->versions[0]->nb_rows; i++) {
        for(int j = 0; j < test->versions[0]->nb_cols; j++) {
            printf("%c", test->versions[0]->matrix[i][j].character);
        }
        printf("\n");
    }
}
