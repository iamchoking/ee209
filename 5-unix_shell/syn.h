#ifndef SYN_h
#define SYN_h

#include "dynarray.h"
#include "lex.h"

enum cmdType{CMD_NORMAL,CMD_BUILTIN};

typedef struct cmd * cmd_t;

// void add_cmd(DynArray_T tokens, DynArray_T cmds, int startI, int endI);
// void free_cmd(void* pcmd);
void print_cmds(DynArray_T cmds);

void delete_cmds(DynArray_T cmds);

int syn(DynArray_T tokens, DynArray_T cmds, char* programName);

char* cmd_name(cmd_t cmd);
char** cmd_argv(cmd_t cmd);
int cmd_len(cmd_t cmd);
int cmd_type(cmd_t cmd);
#endif