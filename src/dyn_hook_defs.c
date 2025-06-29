#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include "intrusive_list.h"
#include "dyn_hook_defs.h"

struct list_head sym_list = LIST_HEAD_INIT(sym_list);
struct list_head hook_def_head = LIST_HEAD_INIT(hook_def_head);
struct list_head interface_list = LIST_HEAD_INIT(interface_list);

struct SymData* add_sym(char* name, uint64_t addr) {
    struct SymData* dat = (struct SymData*)malloc(sizeof(struct SymData));
    if (dat == NULL) {
        printf("failed to alloc SymDat\n");
        abort();
    }
    memset(dat, 0, sizeof(struct SymData));
    dat->name = strdup(name);
    dat->addr = addr;
    list_add_tail(&dat->node, &sym_list);
    return dat;
}


void print_syms() {
    struct list_head* curr_node = NULL;
    struct SymData* sym = NULL;
    list_for_each(curr_node, &sym_list) {
        sym = list_entry(curr_node, struct SymData, node);
        printf("%s @ 0x%0zx\n", sym->name, sym->addr);
    }
}

void print_hook_defs() {
    struct list_head* curr_node = NULL;
    struct HookDef* hook_def = NULL;
    struct HookDefRegs* regs;
    list_for_each(curr_node, &hook_def_head) {
        hook_def = list_entry(curr_node, struct HookDef, node);
        printf("handler %p ", hook_def->handler);
        switch (hook_def->type) {
            case HOOKDEF_NONE:
                printf("Illegal none hook type\n");
                break;
            case HOOKDEF_REGS:
                regs = (struct HookDefRegs*)hook_def->hook_data;
                printf("regs: addr 0x%zx\n", regs->addr);
                break;
            case HOOKDEF_REGADDR:
                break;
            case HOOKDEF_ALLREGS:
                break;
            case HOOKDEF_DUMPADDR:
                break;
            default:
                printf("unknown hook type\n");
                break;
        }
    }
}



struct HookDefRegAddr* new_HookDefRegAddr(uint64_t addr,  uint64_t regno, uint64_t size) {
    struct HookDefRegAddr* res = (struct HookDefRegAddr*)malloc(sizeof(struct HookDefRegAddr));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(struct HookDefRegAddr));
    res->addr = addr;
    res->regno = regno;
    res->size = size;
    return res;
}

struct HookDef* new_HookDef(enum HookDefType hook_type){
    struct HookDef* res = (struct HookDef*)malloc(sizeof(struct HookDef));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(*res));
    res->type = hook_type;
    return res;

}

int register_handler(struct HookDefHandlerInterface* new_handler_int){
    int res = -1;
    struct list_head* curr_node;
    struct HookDefHandlerInterface* curr_hand = NULL;
    list_for_each(curr_node, &interface_list) {
        curr_hand = list_entry(curr_node, struct HookDefHandlerInterface, node);
        if (new_handler_int->cmd_type == curr_hand->cmd_type) {
            printf("handler for cmd %d already exists\n", new_handler_int->cmd_type);
            goto exit;
        }
    }

    list_add_tail(&new_handler_int->node, &interface_list);
    res = 0;
exit:
    return res;
}

void parse_hookdef_sym(int argc, char** argv) {
    uint64_t addr = strtoull(argv[1], NULL, 16);
    add_sym(argv[0], addr);
    //printf("parse sym \"%s\" 0x%0zx\n", argv[0], addr);
    //printf("sym: ptr %p name \"%s\" 0x%0zx\n", (void*)sym, sym->name, sym->addr);
    return;
}

void parse_hookdef_regs(int argc, char** argv) {
    printf("parse regs\n");
    return;
}

void parse_hookdef_regaddr(int argc, char** argv) {
    struct HookDef* hook_def = NULL;
    struct HookDefRegAddr* args = NULL;
    uint64_t addr;
    uint64_t regno = strtoul(argv[1], NULL, 10); // TODO: actually find regno from reg name
    uint64_t size = strtoul(argv[2], NULL, 10);
    if (0 == strncasecmp(argv[0], "0x", sizeof("0x")-1)) {
        addr = strtoull(argv[0], NULL, 16);
        args = new_HookDefRegAddr(addr, regno, size);
        if (args == NULL) {
            abort();
        }
        hook_def = new_HookDef(HOOKDEF_REGADDR);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)args;
        list_add_tail(&hook_def->node, &hook_def_head);
        return;
    }
    struct list_head* curr_node = NULL;
    struct SymData* sym = NULL;
    size_t sym_str_len = strlen(argv[0]);
    list_for_each(curr_node, &sym_list) {
        sym = list_entry(curr_node, struct SymData, node);
        //printf("curr node %p sym %p sym name %s\n", curr_node, sym, sym->name);
        //fflush(stdout);
        if (0 != strncmp(sym->name, argv[0], sym_str_len+1)) {
            continue;
        }
        args = new_HookDefRegAddr(sym->addr, regno, size);
        if (args == NULL) {
            abort();
        }
        hook_def = new_HookDef(HOOKDEF_REGADDR);
        if (hook_def == NULL) {
            abort();
        }
        hook_def->hook_data = (void*)args;
        list_add_tail(&hook_def->node, &hook_def_head);
    }
    //printf("parse regaddr\n");
    return;
}

void parse_hookdef_allregs(int argc, char** argv) {
    printf("parse allregs\n");
    return;
}

void parse_hookdef_dumpaddr(int argc, char** argv) {
    printf("parse dumpaddr\n");
    return;
}


struct HookDefHandlerInterface sym_handler_intf = {
    .parse_req = {
        .token = "sym",
        .nargs = 2,
        .parse_func = &parse_hookdef_sym,
        .arg_desc = "<name>,<addr>"
    },  // <name>,<addr>
    .cmd_type = 4,
};


void init_hook_def_sym() {
    register_handler(&sym_handler_intf);
}



static struct HookdefParseReq parsereqs[] = {
    { .token = "regs", .nargs = 2,
      .parse_func = &parse_hookdef_regs,
      .arg_desc = "<addr|sym>,<reglist>"},       // <addr|sym>,<reglist>
    { .token = "regaddr", .nargs = 3,
      .parse_func = &parse_hookdef_regaddr,
      .arg_desc = "<addr|sym>,<reg>,<size>"},    // <addr|sym>,<reg>,<size>
    { .token = "allregs", .nargs = 1,
      .parse_func = &parse_hookdef_allregs,
      .arg_desc = "<addr|sym>"},    // <addr|sym>
    { .token = "dumpaddr", .nargs = 3,
      .parse_func = &parse_hookdef_dumpaddr,
      .arg_desc = "<addr|sym>,<addr>,<size>"},   // <addr|sym>,<addr>,<size>
};

void parse_hookdef_cmd(int argc, char** argv){
    int currind = 0;
    struct HookdefParseReq* parsereq;
    while ((currind < argc-1) && argv[currind] != NULL) {
        //printf("currind %d argc-1 %d\n", currind, argc-1);
        //fflush(stdout);
        parsereq = NULL;
        struct list_head* curr_node = NULL;
        struct HookDefHandlerInterface* intr = NULL;
        list_for_each(curr_node, &interface_list) {
            intr = list_entry(curr_node, struct HookDefHandlerInterface, node);
            // +1 to handle cases where the start of a command name is the same as the entire name of another command
            if (0 == strncasecmp(intr->parse_req.token, argv[currind], strlen(intr->parse_req.token)+1)) {
                parsereq = &intr->parse_req;
                break;
            }
        }

        currind++;
        if (parsereq == NULL) {
            printf("invalid hook type %s\n", argv[currind-1]);
            fflush(stdout);
            continue;
        }

        if (parsereq->nargs + currind > argc) {
            printf("not enough args to parse %s\n", argv[currind-1]);
            fflush(stdout);
            break;
        }

        if (parsereq->parse_func != NULL) {
            parsereq->parse_func(parsereq->nargs, &argv[currind]);
        }
        currind += parsereq->nargs;
    }
}

