#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "dyn_hook_defs.h"
#include "hook_def_args.h"
#include "hook_def_regaddr.h"
#include "hook_def_regs.h"
#include "parse_utils.h"
#include "hook_def_syms.h"
#include "hook_def_dumpaddr.h"

struct HookDefHandlerInterface intf_sym;
struct HookDefHandlerInterface intf_args;
struct HookDefHandlerInterface intf_regaddr;
struct HookDefHandlerInterface intf_regs;
struct HookDefHandlerInterface intf_dumpaddr;

int main(int argc, char* argv[]) {
    init_hook_def_sym(&intf_sym);
    init_hook_def_args(&intf_args);
    init_hook_def_regaddr(&intf_regaddr);
    init_hook_def_regs(&intf_regs);
    init_hook_def_dumpaddr(&intf_dumpaddr);
    printf("parse reqs\n");
    print_parse_reqs();
    printf("\n");
    parse_hookdef_cmd(argc-1, &argv[1]);
    printf("syms:\n");
    print_syms();
    printf("\n");
    printf("hook defs:\n");
    print_hook_defs();
    printf("\n");
    return 0;
}
