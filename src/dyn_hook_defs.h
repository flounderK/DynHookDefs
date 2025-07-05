#ifndef DYN_HOOK_DEFS_H
#define DYN_HOOK_DEFS_H
#include <stdint.h>
#include "intrusive_list.h"

struct HookDef;

// structure for registering new handlers for parsing values to a HookDef object in different engines
struct HookDefHandlerInterface {
    //void* data;
    //void* (*create)();
    void (*print)(struct HookDef*);
    void (*destroy)(struct HookDef*);
    void (*enable)(struct HookDef*);
    void* handler_func;
};

struct HookDefParseReq {
    struct list_head node;
    char* token;
    void (*parse_func)(struct HookDefHandlerInterface* intf, int argc, char** argv);
    char* arg_desc;
    struct HookDefHandlerInterface* intf;
    int nargs;
};

enum HookDefType {
    HOOKDEF_NONE = 0,
    HOOKDEF_ARGS,
    HOOKDEF_REGS,
    HOOKDEF_REGADDR,
    HOOKDEF_SYM,
    HOOKDEF_ALLREGS,
    HOOKDEF_DUMPADDR,
};

struct HookDef {
    struct list_head node;
    void* hook_data;  // pointer to struct like `struct HookDefArgs*`
    void* handler;    // function for handling
    struct HookDefHandlerInterface* intf;
};

struct HookDefAllRegs {
    uint64_t addr;
};


int register_parse_handler(struct HookDefParseReq* new_handler_int);
void parse_hookdef_cmd(int argc, char* argv[]);

extern struct list_head hook_def_head;
extern struct list_head parse_req_list;
void print_hook_defs();
struct HookDef* new_HookDef(struct HookDefHandlerInterface* hook_def_intf);
void destroy_HookDef(struct HookDef* hook_def);
#endif // DYN_HOOK_DEFS_H
