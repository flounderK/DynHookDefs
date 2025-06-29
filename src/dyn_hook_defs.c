#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "intrusive_list.h"
#include "dyn_hook_defs.h"

struct list_head sym_list = LIST_HEAD_INIT(sym_list);
struct list_head hook_def_head = LIST_HEAD_INIT(hook_def_head);

void add_sym(char* name, uint64_t addr) {
    struct SymData* dat = (struct SymData*)malloc(sizeof(struct SymData));
    if (dat == NULL) {
        printf("failed to alloc SymDat\n");
        abort();
    }
    memset(dat, 0, sizeof(struct SymData));
    dat->name = strdup(name);
    dat->addr = addr;
    list_add_tail(&dat->node, &sym_list);
    return;
}


void print_syms() {
    struct list_head* curr_node = NULL;
    struct SymData* sym = NULL;
    list_for_each(curr_node, &sym_list) {
        sym = list_entry(curr_node, struct SymData, node);
        printf("%s @ 0x%0zu\n", sym->name, sym->addr);
    }

}


struct HookDefArgs* new_HookDefArgs(uint64_t addr, uint32_t nregs) {
    struct HookDefArgs* res = (struct HookDefArgs*)malloc(sizeof(struct HookDefArgs));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(struct HookDefArgs));
    res->addr = addr;
    res->nregs = nregs;
    return res;
}

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

struct HookDef* new_HookDef(enum HookDefType hook_type){
    struct HookDef* res = (struct HookDef*)malloc(sizeof(struct HookDef));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(*res));
    res->type = hook_type;
    return res;

}
