#ifndef __ARCH_UTILS_H__
#define __ARCH_UTILS_H__
#include <stdint.h>

// Singleton Run Time interface for interacting with arch-specific things
struct ArchUtilsOps {
    uint32_t (*regname_to_regno)(char*);
    const char* (*regno_to_reg_name)(uint32_t);
    // can't do argno to regno because some architectures pass args past 3 on the stack
    uint64_t (*argno_to_arg_value)(uint32_t);
    uint64_t (*get_current_pc)(void*);
    uint64_t (*get_return_pc)(void*);

};

#endif //__ARCH_UTILS_H__
