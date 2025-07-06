#define _GNU_SOURCE
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "hook_def_regs.h"
#include "dyn_hook_defs.h"
#include "hook_def_syms.h"
#include "arch_utils.h"
#include "parse_utils.h"

struct HookDefRegs* new_HookDefRegs(uint64_t addr, struct RegEntry* reglist, int num_regentries) {
    struct HookDefRegs* res = (struct HookDefRegs*)malloc(sizeof(struct HookDefRegs));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(struct HookDefRegs));
    res->addr = addr;
    res->regentries = reglist;
    res->num_regentries = num_regentries;
    return res;
}


static void print_regs(struct HookDef* hook_def){
    struct HookDefRegs* regs;
    regs = (struct HookDefRegs*)hook_def->hook_data;
    printf("regs: addr 0x%0zx ", regs->addr);
    if (regs->regentries) {
        for (int i = 0; i < regs->num_regentries; i++) {
            printf("%s: %u ", regs->regentries[i].regname, regs->regentries[i].regno);
        }
    }
    printf("\n");
}

struct RegEntry* parse_reglist_to_reg_entry_array(int argc, char** argv) {
    size_t arraybytesize = sizeof(struct RegEntry)*(argc+1);
    struct RegEntry* res = (struct RegEntry*)malloc(arraybytesize);
    if (res == NULL) {
        printf("Unable to allocate\n");
        abort();
    }
    memset(res, 0, arraybytesize);

    struct ArchOps* arch_ops = get_arch_ops();

    for (int i = 0; i < argc; i++) {
        //printf("copying %p\n", argv[i]);
        //printf("copying %s\n", argv[i]);
        res[i].regname = strdup(argv[i]);
        res[i].regno = arch_ops->regname_to_regno(argv[i]);
    }
    return res;
}

static void parse_hookdef_regs(struct HookDefParseReq* parse_req, int argc, char** argv) {
    struct HookDef* hook_def = NULL;
    struct HookDefRegs* regs = NULL;
    uint64_t addr;
    char* mutable_reglist_str = strdup(argv[1]);
    replace_char(mutable_reglist_str, ',', ' ');
    int reglist_argc = 0;
    char** reglist_argv = NULL;
    parse_to_argv_and_argc(mutable_reglist_str, &reglist_argc, &reglist_argv);
    /*
    printf("reglist argc %d\n", reglist_argc);
    for (int i = 0; i < reglist_argc; i++) {
        printf("i %d\n", i);
        printf("argv[%d] %p\n", i, reglist_argv[i]);
        printf("argv[%d] %s\n", i, reglist_argv[i]);
        fflush(stdout);
    }
    */
    if (0 == strncasecmp(argv[0], "0x", sizeof("0x")-1)) {
        addr = strtoull(argv[0], NULL, 16);
        struct RegEntry* reglist = parse_reglist_to_reg_entry_array(reglist_argc, reglist_argv);
        regs = new_HookDefRegs(addr, reglist, reglist_argc);
        if (regs == NULL) {
            abort();
        }
        hook_def = new_HookDef(parse_req);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)regs;
        list_add_tail(&hook_def->node, &hook_def_head);
        goto exit;
    }
    struct list_head* curr_node = NULL;
    struct SymData* sym = NULL;
    size_t sym_str_len = strlen(argv[0]);
    list_for_each(curr_node, &sym_list) {
        sym = list_entry(curr_node, struct SymData, node);
        if (0 != strncmp(sym->name, argv[0], sym_str_len+1)) {
            continue;
        }
        struct RegEntry* reglist = parse_reglist_to_reg_entry_array(reglist_argc, reglist_argv);
        regs = new_HookDefRegs(sym->addr, reglist, reglist_argc);
        if (regs == NULL) {
            abort();
        }
        hook_def = new_HookDef(parse_req);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)regs;
        list_add_tail(&hook_def->node, &hook_def_head);
    }
exit:
    if (mutable_reglist_str != NULL) {
        free(mutable_reglist_str);
    }
    if (reglist_argv != NULL) {
        for (int i = 0; i < reglist_argc; i++) {
            if (reglist_argv[i] != NULL) {
                free(reglist_argv[i]);
            }
        }
        free(reglist_argv);
    }

    return;
}


struct HookDefHandlerInterface regs_handler_intf;

struct HookDefParseReq regs_parse_req = {
    .token = "regs", .nargs = 2,
    .parse_func = &parse_hookdef_regs,
    .arg_desc = "<addr|sym>,<reglist>",
    .print = &print_regs
};

void init_hook_def_regs(struct HookDefHandlerInterface* intf) {
    struct HookDefParseReq* parse_req = copy_HookDefParseReq(&regs_parse_req);
    parse_req->intf = intf;
    register_parse_handler(parse_req);
}
