#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "dyn_hook_defs.h"

int main(int argc, char* argv[]) {
    parse_hookdef_cmd(argc-1, &argv[1]);
    printf("syms:\n");
    print_syms();
    printf("hook defs:\n");
    print_hook_defs();
    return 0;
}
