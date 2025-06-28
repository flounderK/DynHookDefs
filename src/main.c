#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "hookdef_parser.h"

int main(int argc, char* argv[]) {
    parse_hookdef_cmd(argc-1, &argv[1]);
    return 0;
}
