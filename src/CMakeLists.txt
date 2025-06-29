

add_library(dyn_hook_def_obj OBJECT dyn_hook_defs.c)
add_executable(main main.c $<TARGET_OBJECTS:dyn_hook_def_obj> )
