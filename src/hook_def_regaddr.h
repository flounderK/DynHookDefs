#ifndef HOOK_DEF_REGADDR_H
#define HOOK_DEF_REGADDR_H
#include <stdint.h>
#include "dyn_hook_defs.h"

struct HookDefRegAddr {
    uint64_t addr;
    uint64_t regno;
    uint64_t size;
};


void init_hook_def_regaddr(struct HookDefHandlerInterface*);
#endif // HOOK_DEF_REGADDR_H
