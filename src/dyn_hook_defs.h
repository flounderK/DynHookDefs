#ifndef DYN_HOOK_DEFS_H
#define DYN_HOOK_DEFS_H
#include <stdint.h>
#include "intrusive_list.h"

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


extern struct list_head sym_list;
extern struct list_head hook_def_head;
void add_sym(char* name, uint64_t addr);
void print_syms();
struct HookDefArgs* new_HookDefArgs(uint64_t addr, uint32_t nregs);
struct HookDef* new_HookDef(enum HookDefType hook_type);
struct HookDefRegAddr* new_HookDefRegAddr(uint64_t addr,  uint64_t regno, uint64_t size);
#endif // DYN_HOOK_DEFS_H
