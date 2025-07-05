#include <stdint.h>
#include <unistd.h>
#include "arch_utils.h"

static struct ArchOps* ops = NULL;

void register_arch_ops(struct ArchOps* new_ops) {
    ops = new_ops;
}

struct ArchOps* get_arch_ops() {
    return ops;
}
