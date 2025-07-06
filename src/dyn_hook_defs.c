#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include "intrusive_list.h"
#include "dyn_hook_defs.h"

struct list_head hook_def_head = LIST_HEAD_INIT(hook_def_head);
struct list_head parse_req_list = LIST_HEAD_INIT(parse_req_list);

void print_hook_defs() {
    struct list_head* curr_node = NULL;
    struct HookDef* hook_def = NULL;
    list_for_each(curr_node, &hook_def_head) {
        hook_def = list_entry(curr_node, struct HookDef, node);
        printf("handler %p ", hook_def->handler);
        if (hook_def->parse_req->print != NULL) {
            hook_def->parse_req->print(hook_def);
        }
    }
}

struct HookDef* new_HookDef(struct HookDefParseReq* parse_req){
    if (parse_req == NULL) {
        printf("%s hook def intf is null\n", __func__);
        abort();
    }
    struct HookDef* res = (struct HookDef*)malloc(sizeof(struct HookDef));
    if (res == NULL) {
        printf("%s unable to alloc\n", __func__);
        abort();
    }
    memset(res, 0, sizeof(struct HookDef));
    res->parse_req = parse_req;
    res->intf = parse_req->intf;
    return res;
}

void destroy_HookDef(struct HookDef* hook_def) {
    free(hook_def);
    return;
}

int register_parse_handler(struct HookDefParseReq* new_parse_req){
    int res = -1;
    list_add_tail(&new_parse_req->node, &parse_req_list);
    res = 0;
    return res;
}

void parse_hookdef_allregs(struct HookDefParseReq* parse_req, int argc, char** argv) {
    printf("parse allregs\n");
    return;
}

static struct HookDefParseReq parsereqs[] = {
    { .token = "allregs", .nargs = 1,
      .parse_func = &parse_hookdef_allregs,
      .arg_desc = "<addr|sym>"},    // <addr|sym>
};

void parse_hookdef_cmd(int argc, char** argv){
    int currind = 0;
    struct HookDefParseReq* parsereq = NULL;
    while ((currind < argc-1) && argv[currind] != NULL) {
        //printf("currind %d: argv[currind] %s\n", currind, argv[currind]); fflush(stdout);
        parsereq = NULL;
        struct list_head* curr_node = NULL;
        list_for_each(curr_node, &parse_req_list) {
            parsereq = list_entry(curr_node, struct HookDefParseReq, node);
            // +1 to handle cases where the start of a command name is the same as the entire name of another command
            if (0 == strncasecmp(parsereq->token, argv[currind], strlen(parsereq->token)+1)) {
                //parsereq = &intr->parse_req;
                break;
            }
        }

        currind++;
        if (curr_node == &parse_req_list || parsereq == NULL) {
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
            parsereq->parse_func(parsereq, parsereq->nargs, &argv[currind]);
        }
        currind += parsereq->nargs;
    }
}

