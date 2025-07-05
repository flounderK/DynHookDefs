#ifndef HOOK_DEF_REGS_H
#define HOOK_DEF_REGS_H
#include <stdint.h>
#include "intrusive_list.h"

struct RegEntry {
    char* regname;
    uint32_t regno;
};

struct HookDefRegs {
    struct RegEntry* regentries;
    int num_regentries;
    uint64_t addr;
};

void init_hook_def_regs();
#endif //HOOK_DEF_REGS_H
