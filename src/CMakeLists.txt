

add_library(parser_obj OBJECT hookdef_parser.c)
add_executable(main main.c $<TARGET_OBJECTS:parser_obj>)
