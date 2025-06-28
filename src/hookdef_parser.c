#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <unistd.h>
#include "hookdef_parser.h"
#include "intrusive_list.h"
#include "debug_utils.h"


struct SymData {
    struct list_head node;
    char* name;
    uint64_t addr;
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

struct HookDef {
    struct list_head node;
    enum HookDefType type;
};

struct HookdefParseReq {
    char* token;
    struct HookDef* (*parse_func)(int argc, char** argv);
    int nargs;
};

static struct list_head hook_def_head = LIST_HEAD_INIT(hook_def_head);
static struct list_head sym_list = LIST_HEAD_INIT(sym_list);

static void add_sym(char* name, uint64_t addr) {
    struct SymData* dat = (struct SymData*)malloc(sizeof(struct SymData));
    if (dat == NULL) {
        printf("failed to alloc SymDat\n");
        abort();
    }
    memset(dat, 0, sizeof(struct SymData));
    list_add_tail(&dat->node, &sym_list);
    return;
}

struct HookDef* parse_hookdef_sym(int argc, char** argv) {
    uint64_t addr = strtoull(argv[1], NULL, 16);
    add_sym(argv[0], addr);
    printf("parse sym \"%s\" 0x%0zx\n", argv[0], addr);
    return NULL;
}

struct HookDef* parse_hookdef_args(int argc, char** argv) {
    struct HookDef* res = NULL;
    printf("parse args\n");
    return res;
}

struct HookDef* parse_hookdef_regs(int argc, char** argv) {
    struct HookDef* res = NULL;
    printf("parse regs\n");
    return res;
}

struct HookDef* parse_hookdef_regaddr(int argc, char** argv) {
    struct HookDef* res = NULL;
    printf("parse regaddr\n");
    return res;
}

struct HookDef* parse_hookdef_allregs(int argc, char** argv) {
    struct HookDef* res = NULL;
    printf("parse allregs\n");
    return res;
}

struct HookDef* parse_hookdef_dumpaddr(int argc, char** argv) {
    struct HookDef* res = NULL;
    printf("parse dumpaddr\n");
    return res;
}


static struct HookdefParseReq parsereqs[] = {
    [HOOKDEF_NONE] =     { .token = "", .nargs = -1},
    [HOOKDEF_ARGS] =     { .token = "args", .nargs = 2,        // <addr|sym>,<num-regs>
                           .parse_func = &parse_hookdef_args},
    [HOOKDEF_REGS] =     { .token = "regs", .nargs = 2,
                           .parse_func = &parse_hookdef_regaddr},       // <addr|sym>,<reglist>
    [HOOKDEF_REGADDR] =  { .token = "regaddr", .nargs = 3,
                           .parse_func = &parse_hookdef_regaddr},    // <addr|sym>,<reg>,<size>
    [HOOKDEF_SYM] =      { .token = "sym", .nargs = 2,
                           .parse_func = &parse_hookdef_sym},  // <name>,<addr>
    [HOOKDEF_ALLREGS] =  { .token = "allregs", .nargs = 1,
                           .parse_func = &parse_hookdef_allregs},    // <addr|sym>
    [HOOKDEF_DUMPADDR] = { .token = "dumpaddr", .nargs = 3,
                           .parse_func = &parse_hookdef_dumpaddr},   // <addr|sym>,<addr|sym>,<size>
    {.token = NULL, .nargs = -1}
};


void parse_hookdef_cmd(int argc, char** argv){
    int currind = 0;
    struct HookDef* hook_def;
    struct HookdefParseReq* parsereq;
    while ((currind < argc-1) && argv[currind] != NULL) {
        //printf("currind %d argc-1 %d\n", currind, argc-1);
        //fflush(stdout);
        hook_def = NULL;
        parsereq = NULL;
        for (size_t i = 0; i < sizeof(parsereqs)/sizeof(struct HookdefParseReq); i++) {
            // +1 to handle cases where the start of a command name is the same as the entire name of another command
            if (0 == strncasecmp(parsereqs[i].token, argv[currind], strlen(parsereqs[i].token)+1)) {
                parsereq = &parsereqs[i];
                break;
            }
        }

        currind++;
        if (parsereq == NULL) {
            printf("invalid hook type %s\n", argv[currind-1]);
            fflush(stdout);
            continue;
        }

        if (parsereq->nargs + currind > argc) {
            printf("not enough args to parse %s\n", argv[currind-1]);
            fflush(stdout);
            break;
        }

        if (parsereq->parse_func != NULL) {
            hook_def = parsereq->parse_func(parsereq->nargs, &argv[currind]);
            if (hook_def != NULL) {
                list_add_tail(&hook_def->node, &hook_def_head);
            }
        }
        currind += parsereq->nargs;
    }
}
