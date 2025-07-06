#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "intrusive_list.h"
#include "dyn_hook_defs.h"
#include "hook_def_regaddr.h"
#include "hook_def_syms.h"
#include "arch_utils.h"

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


void parse_hookdef_regaddr(struct HookDefParseReq* parse_req, int argc, char** argv) {
    struct HookDef* hook_def = NULL;
    struct HookDefRegAddr* args = NULL;
    uint64_t addr;
    struct ArchOps* arch_ops = get_arch_ops();
    uint64_t regno = arch_ops->regname_to_regno(argv[1]);
    uint64_t size = strtoul(argv[2], NULL, 10);
    if (0 == strncasecmp(argv[0], "0x", sizeof("0x")-1)) {
        addr = strtoull(argv[0], NULL, 16);
        args = new_HookDefRegAddr(addr, regno, size);
        if (args == NULL) {
            abort();
        }
        hook_def = new_HookDef(parse_req);
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
        if (0 != strncmp(sym->name, argv[0], sym_str_len+1)) {
            continue;
        }
        args = new_HookDefRegAddr(sym->addr, regno, size);
        if (args == NULL) {
            abort();
        }
        hook_def = new_HookDef(parse_req);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)args;
        list_add_tail(&hook_def->node, &hook_def_head);
    }
    return;
}


void print_regaddr(struct HookDef* hook_def) {
    struct HookDefRegAddr* regaddr = (struct HookDefRegAddr*)hook_def->hook_data;
    printf("regaddr: addr 0x%0zx regno %zu size %zu\n", regaddr->addr, regaddr->regno, regaddr->size);
}


struct HookDefHandlerInterface regaddr_handler_intf;


struct HookDefParseReq regaddr_parse_req = {
    .token = "regaddr",
    .nargs = 3,
    .parse_func = &parse_hookdef_regaddr,
    .arg_desc = "<addr|sym>,<reg>,<size>",
    .print = &print_regaddr
};


void init_hook_def_regaddr(struct HookDefHandlerInterface* intf) {
    struct HookDefParseReq* parse_req = copy_HookDefParseReq(&regaddr_parse_req);
    parse_req->intf = intf;
    register_parse_handler(parse_req);
}
