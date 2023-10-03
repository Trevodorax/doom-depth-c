#include "src/doom_depth.h"
#include <stdio.h>
/*
int main(int argc, const char * argv[]) {
    return doom_depth();
}
 */

#include "src/cli_utils/cli_utils.h"

int main(void) {
    cli_print(RED, "This is a %s in red %d\n", "string", 42);
    cli_print(GREEN, "This is a %s in green\n", "string");
    cli_print(YELLOW, "This is a %s in yellow\n", "string");
    cli_print(BLUE, "This is a %s in blue\n", "string");
    printf("Full block character: \u2588\u2588\u2588\u2588\n");
    return 0;
}
