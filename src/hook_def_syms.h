#ifndef HOOK_DEF_SYMS_H
#define HOOK_DEF_SYMS_H
#include <stdint.h>
#include "intrusive_list.h"
#include "dyn_hook_defs.h"

struct SymData {
    struct list_head node;
    char* name;
    uint64_t addr;
};

extern struct list_head sym_list;
struct SymData* add_sym(char* name, uint64_t addr);
void print_syms();
void init_hook_def_sym(struct HookDefHandlerInterface* intf);
#endif // HOOK_DEF_SYMS_H
