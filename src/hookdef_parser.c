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
#include "dyn_hook_defs.h"


struct HookDef* parse_hookdef_sym(int argc, char** argv) {
    uint64_t addr = strtoull(argv[1], NULL, 16);
    struct SymData* sym = add_sym(argv[0], addr);
    printf("parse sym \"%s\" 0x%0zx\n", argv[0], addr);
    printf("sym: ptr %p name \"%s\" 0x%0zx\n", (void*)sym, sym->name, sym->addr);
    return NULL;
}

struct HookDef* parse_hookdef_args(int argc, char** argv) {
    struct HookDef* res = NULL;
    struct HookDef* hook_def = NULL;
    struct HookDefArgs* args = NULL;
    uint64_t addr;
    uint32_t nregs = strtoul(argv[1], NULL, 10);
    printf("args argv[0] %s argv[1] %s\n", argv[0], argv[1]);
    if (0 == strncasecmp(argv[0], "0x", sizeof("0x")-1)) {
        addr = strtoull(argv[0], NULL, 16);
        args = new_HookDefArgs(addr, nregs);
        if (args == NULL) {
            abort();
        }
        hook_def = new_HookDef(HOOKDEF_ARGS);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)args;
        list_add_tail(&hook_def->node, &hook_def_head);
        return res;
    }
    struct list_head* curr_node = NULL;
    struct SymData* sym = NULL;
    size_t sym_str_len = strlen(argv[0]);
    list_for_each(curr_node, &sym_list) {
        sym = list_entry(curr_node, struct SymData, node);
        printf("curr node %p sym %p sym name %s sym addr 0x%zx\n", curr_node, sym, sym->name, sym->addr);
        //fflush(stdout);
        if (0 != strncmp(sym->name, argv[0], sym_str_len+1)) {
            continue;
        }
        args = new_HookDefArgs(sym->addr, nregs);
        if (args == NULL) {
            abort();
        }
        hook_def = new_HookDef(HOOKDEF_ARGS);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)args;
        list_add_tail(&hook_def->node, &hook_def_head);
    }
    //printf("parse args\n");
    return res;
}

struct HookDef* parse_hookdef_regs(int argc, char** argv) {
    struct HookDef* res = NULL;
    printf("parse regs\n");
    return res;
}

struct HookDef* parse_hookdef_regaddr(int argc, char** argv) {
    struct HookDef* res = NULL;
    struct HookDef* hook_def = NULL;
    struct HookDefRegAddr* args = NULL;
    uint64_t addr;
    uint64_t regno = strtoul(argv[1], NULL, 10); // TODO: actually find regno from reg name
    uint64_t size = strtoul(argv[2], NULL, 10);
    if (0 == strncasecmp(argv[0], "0x", sizeof("0x")-1)) {
        addr = strtoull(argv[0], NULL, 16);
        args = new_HookDefRegAddr(addr, regno, size);
        if (args == NULL) {
            abort();
        }
        hook_def = new_HookDef(HOOKDEF_REGADDR);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)args;
        list_add_tail(&hook_def->node, &hook_def_head);
        return res;
    }
    struct list_head* curr_node = NULL;
    struct SymData* sym = NULL;
    size_t sym_str_len = strlen(argv[0]);
    list_for_each(curr_node, &sym_list) {
        sym = list_entry(curr_node, struct SymData, node);
        //printf("curr node %p sym %p sym name %s\n", curr_node, sym, sym->name);
        //fflush(stdout);
        if (0 != strncmp(sym->name, argv[0], sym_str_len+1)) {
            continue;
        }
        args = new_HookDefRegAddr(sym->addr, regno, size);
        if (args == NULL) {
            abort();
        }
        hook_def = new_HookDef(HOOKDEF_REGADDR);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)args;
        list_add_tail(&hook_def->node, &hook_def_head);
    }
    //printf("parse regaddr\n");
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
    { .token = "args", .nargs = 2,
      .parse_func = &parse_hookdef_args},    // <addr|sym>,<num-regs>
    { .token = "regs", .nargs = 2,
      .parse_func = &parse_hookdef_regs},       // <addr|sym>,<reglist>
    { .token = "regaddr", .nargs = 3,
      .parse_func = &parse_hookdef_regaddr},    // <addr|sym>,<reg>,<size>
    { .token = "sym", .nargs = 2,
      .parse_func = &parse_hookdef_sym},  // <name>,<addr>
    { .token = "allregs", .nargs = 1,
      .parse_func = &parse_hookdef_allregs},    // <addr|sym>
    { .token = "dumpaddr", .nargs = 3,
      .parse_func = &parse_hookdef_dumpaddr},   // <addr|sym>,<addr>,<size>
};


void parse_hookdef_cmd(int argc, char** argv){
    int currind = 0;
    struct HookdefParseReq* parsereq;
    while ((currind < argc-1) && argv[currind] != NULL) {
        //printf("currind %d argc-1 %d\n", currind, argc-1);
        //fflush(stdout);
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
            parsereq->parse_func(parsereq->nargs, &argv[currind]);
        }
        currind += parsereq->nargs;
    }
}
