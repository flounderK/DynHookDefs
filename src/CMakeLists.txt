

add_library(hook_def_args_obj OBJECT hook_def_args.c)
add_library(hook_def_regaddr_obj OBJECT hook_def_regaddr.c)
add_library(hook_def_syms_obj OBJECT hook_def_syms.c)
add_library(hook_def_regs_obj OBJECT hook_def_regs.c)
add_library(dyn_hook_def_obj OBJECT dyn_hook_defs.c)
add_library(parse_utils_obj OBJECT parse_utils.c)
add_library(arch_utils_obj OBJECT arch_utils.c)
add_executable(main main.c $<TARGET_OBJECTS:dyn_hook_def_obj>
    $<TARGET_OBJECTS:hook_def_args_obj>
    $<TARGET_OBJECTS:hook_def_syms_obj>
    $<TARGET_OBJECTS:hook_def_regs_obj>
    $<TARGET_OBJECTS:hook_def_regaddr_obj>
    $<TARGET_OBJECTS:parse_utils_obj>
    $<TARGET_OBJECTS:arch_utils_obj>
    )
