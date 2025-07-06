#ifndef HOOK_DEF_ARGS_H
#define HOOK_DEF_ARGS_H
#include <stdint.h>
#include "dyn_hook_defs.h"

struct HookDefArgs {
    uint64_t addr;
    uint32_t nregs;
};


void init_hook_def_args(struct HookDefHandlerInterface* intf);
#endif //HOOK_DEF_ARGS_H
