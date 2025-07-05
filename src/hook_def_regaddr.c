#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "intrusive_list.h"
#include "dyn_hook_defs.h"
#include "hook_def_regaddr.h"
#include "hook_def_syms.h"

struct HookDefHandlerInterface regaddr_handler_intf;

struct HookDefRegAddr* new_HookDefRegAddr(uint64_t addr,  uint64_t regno, uint64_t size) {
    struct HookDefRegAddr* res = (struct HookDefRegAddr*)malloc(sizeof(struct HookDefRegAddr));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(struct HookDefRegAddr));
    res->addr = addr;
    res->regno = regno;
    res->size = size;
    return res;
}


void parse_hookdef_regaddr(struct HookDefHandlerInterface* intf, int argc, char** argv) {
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
        hook_def = new_HookDef(intf);
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
        //printf("curr node %p sym %p sym name %s\n", curr_node, sym, sym->name);
        //fflush(stdout);
        if (0 != strncmp(sym->name, argv[0], sym_str_len+1)) {
            continue;
        }
        args = new_HookDefRegAddr(sym->addr, regno, size);
        if (args == NULL) {
            abort();
        }
        hook_def = new_HookDef(intf);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)args;
        list_add_tail(&hook_def->node, &hook_def_head);
    }
    //printf("parse regaddr\n");
    return;
}


void print_regaddr(struct HookDef* hook_def) {
    struct HookDefRegAddr* regaddr = (struct HookDefRegAddr*)hook_def->hook_data;
    printf("regaddr: addr 0x%0zx regno %zu size %zu\n", regaddr->addr, regaddr->regno, regaddr->size);
}


struct HookDefHandlerInterface regaddr_handler_intf = {
    .print = &print_regaddr
};


struct HookDefParseReq regaddr_parse_req = {
    .token = "regaddr",
    .nargs = 3,
    .parse_func = &parse_hookdef_regaddr,
    .arg_desc = "<addr|sym>,<reg>,<size>"
};


void init_hook_def_regaddr() {
    regaddr_parse_req.intf = &regaddr_handler_intf;
    register_parse_handler(&regaddr_parse_req);
}

