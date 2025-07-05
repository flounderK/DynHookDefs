#ifndef __HOOK_DEF_DUMPADDR_H__
#define __HOOK_DEF_DUMPADDR_H__
#include <stdint.h>

struct HookDefDumpAddr {
    uint64_t addr;
    uint64_t dumpaddr;
    uint64_t size;
};

void init_hook_def_dumpaddr();


#endif //__HOOK_DEF_DUMPADDR_H__
