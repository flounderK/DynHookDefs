#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "intrusive_list.h"
#include "dyn_hook_defs.h"

struct list_head sym_list = LIST_HEAD_INIT(sym_list);
struct list_head hook_def_head = LIST_HEAD_INIT(hook_def_head);

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

void print_hook_defs() {
    struct list_head* curr_node = NULL;
    struct HookDef* hook_def = NULL;
    struct HookDefArgs* args;
    struct HookDefRegs* regs;
    list_for_each(curr_node, &hook_def_head) {
        hook_def = list_entry(curr_node, struct HookDef, node);
        printf("handler %p ", hook_def->handler);
        switch (hook_def->type) {
            case HOOKDEF_NONE:
                printf("Illegal none hook type\n");
                break;
            case HOOKDEF_ARGS:
                args = (struct HookDefArgs*)hook_def->hook_data;
                printf("args: addr 0x%0zx nregs %u\n", args->addr, args->nregs);
                break;
            case HOOKDEF_REGS:
                regs = (struct HookDefRegs*)hook_def->hook_data;
                printf("regs: addr 0x%zx\n", regs->addr);
                break;
            case HOOKDEF_REGADDR:
                break;
            case HOOKDEF_ALLREGS:
                break;
            case HOOKDEF_DUMPADDR:
                break;
            default:
                printf("unknown hook type\n");
                break;
        }
    }
}


struct HookDefArgs* new_HookDefArgs(uint64_t addr, uint32_t nregs) {
    struct HookDefArgs* res = (struct HookDefArgs*)malloc(sizeof(struct HookDefArgs));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(struct HookDefArgs));
    printf("args: addr 0x%0zx nregs %u\n", addr, nregs);
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
