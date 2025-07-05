#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "dyn_hook_defs.h"
#include "hook_def_dumpaddr.h"
#include "hook_def_syms.h"

struct HookDefDumpAddr* new_HookDefDumpAddr(uint64_t addr, uint64_t dumpaddr, uint32_t size) {
    struct HookDefDumpAddr* res = (struct HookDefDumpAddr*)malloc(sizeof(struct HookDefDumpAddr));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(struct HookDefDumpAddr));
    res->addr = addr;
    res->dumpaddr = dumpaddr;
    res->size = size;
    return res;
}


static void print_dumpaddr(struct HookDef* hook_def){
    struct HookDefDumpAddr* dat;
    dat = (struct HookDefDumpAddr*)hook_def->hook_data;
    printf("dumpaddr: addr 0x%0zx dumpaddr 0x%0zx size %zu\n", dat->addr, dat->dumpaddr, dat->size);
}

static void parse_hookdef_dumpaddr(struct HookDefHandlerInterface* intf, int argc, char** argv) {
    struct HookDef* hook_def = NULL;
    struct HookDefDumpAddr* dumpaddr = NULL;
    uint64_t addr;
    uint64_t dumpaddr_int = strtoul(argv[1], NULL, 16);
    uint64_t dumpaddr_size = strtoul(argv[2], NULL, 10);
    if (0 == strncasecmp(argv[0], "0x", sizeof("0x")-1)) {
        addr = strtoull(argv[0], NULL, 16);
        dumpaddr = new_HookDefDumpAddr(addr, dumpaddr_int, dumpaddr_size);
        if (dumpaddr == NULL) {
            abort();
        }
        hook_def = new_HookDef(intf);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)dumpaddr;
        list_add_tail(&hook_def->node, &hook_def_head);
        return;
    }
    struct list_head* curr_node = NULL;
    struct SymData* sym = NULL;
    size_t sym_str_len = strlen(argv[0]);
    list_for_each(curr_node, &sym_list) {
        sym = list_entry(curr_node, struct SymData, node);
        if (0 != strncmp(sym->name, argv[0], sym_str_len+1)) {
            continue;
        }
        dumpaddr = new_HookDefDumpAddr(sym->addr, dumpaddr_int, dumpaddr_size);
        if (dumpaddr == NULL) {
            abort();
        }
        hook_def = new_HookDef(intf);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)dumpaddr;
        list_add_tail(&hook_def->node, &hook_def_head);
    }
    return;
}

struct HookDefHandlerInterface dumpaddr_handler_intf = {
    .print = &print_dumpaddr
};

struct HookDefParseReq dumpaddr_parse_req = {
    .token = "dumpaddr", .nargs = 3,
    .parse_func = &parse_hookdef_dumpaddr,
    .arg_desc = "<addr|sym>,<addr>,<size>"
};   // <addr|sym>,<addr>,<size>


void init_hook_def_dumpaddr() {
    dumpaddr_parse_req.intf = &dumpaddr_handler_intf;
    register_parse_handler(&dumpaddr_parse_req);
}


