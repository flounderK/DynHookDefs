#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "hook_def_args.h"
#include "dyn_hook_defs.h"


static void print_args(struct HookDef* hook_def){
    struct HookDefArgs* args;
    args = (struct HookDefArgs*)hook_def->hook_data;
    printf("args: addr 0x%0zx nregs %u\n", args->addr, args->nregs);
}

static void parse_hookdef_args(int argc, char** argv) {
    //struct HookDef* res = NULL;
    struct HookDef* hook_def = NULL;
    struct HookDefArgs* args = NULL;
    uint64_t addr;
    uint32_t nregs = strtoul(argv[1], NULL, 10);
    //printf("args argv[0] %s argv[1] %s\n", argv[0], argv[1]);
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
        return;
    }
    struct list_head* curr_node = NULL;
    struct SymData* sym = NULL;
    size_t sym_str_len = strlen(argv[0]);
    list_for_each(curr_node, &sym_list) {
        sym = list_entry(curr_node, struct SymData, node);
        //printf("curr node %p sym %p sym name %s sym addr 0x%zx\n", curr_node, sym, sym->name, sym->addr);
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
    return;
}



struct HookDefHandlerInterface args_handler_intf = {
    .parse_req = {   // <addr|sym>,<num-regs>
        .token = "args",
        .nargs = 2,
        .parse_func = &parse_hookdef_args,
        .arg_desc = "<addr|sym>,<num-regs>"
    },
    .print = &print_args,
    .cmd_type = 1,
};


void init_hook_def_args() {
    register_handler(&args_handler_intf);
}

