#include "src/doom_depth.h"
#include "src/cli_utils/cli_utils.h"

#include <stdio.h>

int main(int argc, char * argv[]) {
    ui_type_t ui_type = get_ui_type(argv[1]);

    doom_depth_main doom_depth = doom_depth_factory(ui_type);

    return doom_depth();
}
