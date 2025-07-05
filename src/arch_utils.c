#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include "arch_utils.h"

static uint32_t default_regname_to_regno(char* regname) {
    printf("WARNING: default impl of %s, you need to register an ArchOps\n", __func__);
    return 0;
}

static char* default_regno_to_reg_name(uint32_t regno){
    printf("WARNING: default impl of %s, you need to register an ArchOps\n", __func__);
    return NULL;
}

static uint64_t default_argno_to_arg_value(uint32_t argno) {
    printf("WARNING: default impl of %s, you need to register an ArchOps\n", __func__);
    return 0;
}

static uint64_t default_get_current_pc(void* inst) {
    printf("WARNING: default impl of %s, you need to register an ArchOps\n", __func__);
    return 0;
}

static uint64_t default_get_return_pc(void* inst) {
    printf("WARNING: default impl of %s, you need to register an ArchOps\n", __func__);
    return 0;
}

static struct ArchOps default_arch_ops = {
    .regname_to_regno = &default_regname_to_regno,
    .regno_to_reg_name = &default_regno_to_reg_name,
    .argno_to_arg_value = &default_argno_to_arg_value,
    .get_current_pc = &default_get_current_pc,
    .get_return_pc = &default_get_return_pc,
};


static struct ArchOps* ops = &default_arch_ops;

void register_arch_ops(struct ArchOps* new_ops) {
    ops = new_ops;
}

struct ArchOps* get_arch_ops() {
    return ops;
}
