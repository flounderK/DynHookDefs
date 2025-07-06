

add_library(hook_def_obj OBJECT
    dyn_hook_defs.c
    hook_def_syms.c
    hook_def_args.c
    hook_def_allregs.c
    hook_def_regaddr.c
    hook_def_regs.c
    hook_def_dumpaddr.c
    parse_utils.c
    arch_utils.c
)
add_executable(main main.c $<TARGET_OBJECTS:hook_def_obj>)
