#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dyn_hook_defs.h"
#include "hook_def_syms.h"
#include "intrusive_list.h"

struct list_head sym_list = LIST_HEAD_INIT(sym_list);

struct SymData* add_sym(char* name, uint64_t addr) {
    struct SymData* dat = (struct SymData*)malloc(sizeof(struct SymData));
    if (dat == NULL) {
        printf("failed to alloc SymDat\n");
        abort();
    }
    memset(dat, 0, sizeof(struct SymData));
    dat->name = strdup(name);
    dat->addr = addr;
    list_add_tail(&dat->node, &sym_list);
    return dat;
}


void print_syms() {
    struct list_head* curr_node = NULL;
    struct SymData* sym = NULL;
    list_for_each(curr_node, &sym_list) {
        sym = list_entry(curr_node, struct SymData, node);
        printf("%s @ 0x%0zx\n", sym->name, sym->addr);
    }
}

void parse_hookdef_sym(struct HookDefParseReq* parse_req, int argc, char** argv) {
    uint64_t addr = strtoull(argv[1], NULL, 16);
    add_sym(argv[0], addr);
    //printf("parse sym \"%s\" 0x%0zx\n", argv[0], addr);
    //printf("sym: ptr %p name \"%s\" 0x%0zx\n", (void*)sym, sym->name, sym->addr);
    return;
}

struct HookDefHandlerInterface sym_handler_intf;

struct HookDefParseReq sym_parse_req = {
    .token = "sym",
    .nargs = 2,
    .parse_func = &parse_hookdef_sym,
    .arg_desc = "<name>,<addr>",
    .intf = &sym_handler_intf
};

void init_hook_def_sym() {
    register_parse_handler(&sym_parse_req);
}

