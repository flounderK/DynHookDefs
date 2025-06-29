#ifndef DYN_HOOK_DEFS_H
#define DYN_HOOK_DEFS_H
#include <stdint.h>
#include "intrusive_list.h"


struct HookdefParseReq {
    char* token;
    void (*parse_func)(int argc, char** argv);
    char* arg_desc;
    int nargs;
};

struct HookDef;

// structure for registering new handlers for parsing values to a HookDef object in different engines
struct HookDefHandlerInterface {
    struct list_head node;
    struct HookdefParseReq parse_req;
    //void* data;
    void* (*create_data)();
    void (*print_data)(void* data);
    void (*destroy_data)(void* data);
    void (*enable_hook)(struct HookDef*);
    void* handler_func;
    int cmd_type;
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

struct SymData {
    struct list_head node;
    char* name;
    uint64_t addr;
};

struct HookDef {
    struct list_head node;
    void* hook_data;  // pointer to struct like `struct HookDefArgs*`
    void* handler;    // function for handling
    struct HookDefHandlerInterface* handler_interface;
    enum HookDefType type;
};

struct HookDefArgs {
    uint64_t addr;
    uint32_t nregs;
};

struct RegEntry {
    struct list_head node;
    char* name;
    uint64_t regno;
};

struct HookDefRegs {
    uint64_t addr;
    struct list_head reglist_head; // list of RegEntry
};

struct HookDefRegAddr {
    uint64_t addr;
    uint64_t regno;
    uint64_t size;
};


struct HookDefAllRegs {
    uint64_t addr;
};


struct HookDefDumpAddr {
    uint64_t addr;
    uint64_t dumpaddr;
    uint64_t size;
};




int register_handler(struct HookDefHandlerInterface* handler_int);
void parse_hookdef_cmd(int argc, char* argv[]);

extern struct list_head sym_list;
extern struct list_head hook_def_head;
struct SymData* add_sym(char* name, uint64_t addr);
void print_syms();
void print_hook_defs();
struct HookDefArgs* new_HookDefArgs(uint64_t addr, uint32_t nregs);
struct HookDef* new_HookDef(enum HookDefType hook_type);
struct HookDefRegAddr* new_HookDefRegAddr(uint64_t addr,  uint64_t regno, uint64_t size);
#endif // DYN_HOOK_DEFS_H
