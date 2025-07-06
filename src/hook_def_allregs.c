#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "hook_def_allregs.h"
#include "dyn_hook_defs.h"
#include "hook_def_syms.h"

struct HookDefAllRegs* new_HookDefAllRegs(uint64_t addr) {
    struct HookDefAllRegs* res = (struct HookDefAllRegs*)malloc(sizeof(struct HookDefAllRegs));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(struct HookDefAllRegs));
    //printf("allregs: addr 0x%0zx nregs %u\n", addr, nregs);
    res->addr = addr;
    return res;
}


static void print_allregs(struct HookDef* hook_def){
    struct HookDefAllRegs* dat;
    dat = (struct HookDefAllRegs*)hook_def->hook_data;
    printf("allregs: addr 0x%0zx\n", dat->addr);
}

static void parse_hookdef_allregs(struct HookDefParseReq* parse_req, int argc, char** argv) {
    //struct HookDef* res = NULL;
    struct HookDef* hook_def = NULL;
    struct HookDefAllRegs* dat = NULL;
    uint64_t addr;
    if (0 == strncasecmp(argv[0], "0x", sizeof("0x")-1)) {
        addr = strtoull(argv[0], NULL, 16);
        dat = new_HookDefAllRegs(addr);
        if (dat == NULL) {
            abort();
        }
        hook_def = new_HookDef(parse_req);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)dat;
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
        dat = new_HookDefAllRegs(sym->addr);
        if (dat == NULL) {
            abort();
        }
        hook_def = new_HookDef(parse_req);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)dat;
        list_add_tail(&hook_def->node, &hook_def_head);
    }
    return;
}

struct HookDefHandlerInterface allregs_handler_intf;

struct HookDefParseReq allregs_parse_req = {   // <addr|sym>,<num-regs>
    .token = "allregs",
    .nargs = 1,
    .parse_func = &parse_hookdef_allregs,
    .arg_desc = "<addr|sym>",
    .print = &print_allregs
};

void init_hook_def_allregs(struct HookDefHandlerInterface* intf) {
    struct HookDefParseReq* parse_req = copy_HookDefParseReq(&allregs_parse_req);
    parse_req->intf = intf;
    register_parse_handler(parse_req);
}

