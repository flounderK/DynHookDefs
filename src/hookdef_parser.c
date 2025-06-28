#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include "hookdef_parser.h"
#include "intrusive_list.h"
#include "debug_utils.h"


struct HookDef {
    struct list_head node;


};

enum HookDefType {
    HOOKDEF_NONE = 0,
    HOOKDEF_ARGS,
    HOOKDEF_REGS,
    HOOKDEF_REGADDR,
    HOOKDEF_SYM,
    HOOKDEF_ALLREGS,
    HOOKDEF_DUMPADDR,
};

struct HookdefParseReq {
    char* token;
    int nargs;
};

static struct HookdefParseReq parsereqs[] = {
    [HOOKDEF_NONE] =     { .token = "", .nargs = -1},
    [HOOKDEF_ARGS] =     { .token = "args", .nargs = 2},     // <addr|sym>,<num-regs>
    [HOOKDEF_REGS] =     { .token = "regs", .nargs = 2},     // <addr|sym>,<reglist>
    [HOOKDEF_REGADDR] =  { .token = "regaddr", .nargs = 3},  // <addr|sym>,<reg>,<size>
    [HOOKDEF_SYM] =      { .token = "sym", .nargs = 2},      // <name>,<addrlist>
    [HOOKDEF_ALLREGS] =  { .token = "allregs", .nargs = 1},  // <addr|sym>
    [HOOKDEF_DUMPADDR] = { .token = "dumpaddr", .nargs = 3}, // <addr|sym>,<addr|sym>,<size>
    {.token = NULL, .nargs = -1}
};


void parse_hookdef_cmd(int argc, char** argv){

    int currind = 0;
    while (currind <= argc-1 && argv[currind] != NULL) {
        enum HookDefType hook_type = HOOKDEF_NONE;
        struct HookdefParseReq* parsereq = NULL;
        for (int i = 0; parsereqs[i].token != NULL; i++) {
            // +1 to handle cases where the start of a command name is the same as the entire name of another command
            int b = strncasecmp( parsereqs[i].token, argv[currind], strlen(parsereqs[i].token)+1);
            //printf("%s b = %d\n", parsereqs[i].token, b);
            if (0 == b) {
                hook_type = (enum HookDefType)i;
                parsereq = &parsereqs[i];
                break;
            }
        }

        currind++;
        if (hook_type == HOOKDEF_NONE) {
            printf("invalid hook type %s\n", argv[currind]);
            return;
        }

        switch (hook_type) {
            case HOOKDEF_ARGS:
                //parse_hookdef_args(argc - 1, &argv[currind]);
                printf("args\n");
                break;
            default:
                printf("Unhandled hook type\n");
                return;
                break;
        }


    }
}
