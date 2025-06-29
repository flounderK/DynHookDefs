#ifndef __HOOKDEF_PARSER_H__
#define __HOOKDEF_PARSER_H__
#include <unistd.h>
#include <stdlib.h>
//#include "intrusive_list.h"

struct HookdefParseReq {
    char* token;
    struct HookDef* (*parse_func)(int argc, char** argv);
    int nargs;
};

void parse_hookdef_cmd(int argc, char* argv[]);


#endif //ifndef __HOOKDEF_PARSER_H__
