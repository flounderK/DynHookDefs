#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "hook_def_regs.h"
#include "dyn_hook_defs.h"

struct HookDefHandlerInterface regs_handler_intf;

struct HookDefRegs* new_HookDefRegs(uint64_t addr) {
    struct HookDefRegs* res = (struct HookDefRegs*)malloc(sizeof(struct HookDefRegs));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(struct HookDefRegs));
    //printf("args: addr 0x%0zx nregs %u\n", addr, nregs);
    res->addr = addr;
    return res;
}


static void print_regs(struct HookDef* hook_def){
    struct HookDefRegs* regs;
    regs = (struct HookDefRegs*)hook_def->hook_data;
    printf("regs: addr 0x%0zx\n", regs->addr);
}

static void parse_hookdef_regs(struct HookDefHandlerInterface* intf, int argc, char** argv) {
    //struct HookDef* res = NULL;
    struct HookDef* hook_def = NULL;
    struct HookDefRegs* regs = NULL;
    uint64_t addr;
    //printf("regs argv[0] %s argv[1] %s\n", argv[0], argv[1]);
    if (0 == strncasecmp(argv[0], "0x", sizeof("0x")-1)) {
        addr = strtoull(argv[0], NULL, 16);
        regs = new_HookDefRegs(addr);
        if (regs == NULL) {
            abort();
        }
        hook_def = new_HookDef(intf);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)regs;
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
        regs = new_HookDefRegs(sym->addr);
        if (regs == NULL) {
            abort();
        }
        hook_def = new_HookDef(intf);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)regs;
        list_add_tail(&hook_def->node, &hook_def_head);
    }
    //printf("parse regs\n");
    return;
}



struct HookDefParseReq regs_parse_req = {
    .token = "regs", .nargs = 2,
    .parse_func = &parse_hookdef_regs,
    .arg_desc = "<addr|sym>,<reglist>"
};


struct HookDefHandlerInterface regs_handler_intf = {
    .print = &print_regs
};


void init_hook_def_regs() {
    regs_parse_req.intf = &regs_handler_intf;
    register_parse_handler(&regs_parse_req);
}

