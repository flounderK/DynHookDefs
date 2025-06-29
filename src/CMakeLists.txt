

add_library(dyn_hook_def_obj OBJECT dyn_hook_defs.c)
add_library(parser_obj OBJECT hookdef_parser.c)
add_executable(main main.c $<TARGET_OBJECTS:parser_obj> $<TARGET_OBJECTS:dyn_hook_def_obj> )
