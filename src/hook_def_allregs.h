#ifndef HOOK_DEF_ALLREGS_H
#define HOOK_DEF_ALLREGS_H
#include <stdint.h>
#include "dyn_hook_defs.h"

struct HookDefAllRegs {
    uint64_t addr;
};

void init_hook_def_allregs(struct HookDefHandlerInterface* intf);
#endif //HOOK_DEF_ALLREGS_H
