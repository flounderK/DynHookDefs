#ifndef HOOK_DEF_ARGS_H
#define HOOK_DEF_ARGS_H
#include <stdint.h>

struct HookDefArgs {
    uint64_t addr;
    uint32_t nregs;
};


struct HookDefArgs* new_HookDefArgs(uint64_t addr, uint32_t nregs);

void init_hook_def_args();
#endif //HOOK_DEF_ARGS_H
