#ifndef EXEC_h
#define EXEC_h

#include "dynarray.h"
#include "lex.h"
#include "syn.h"

int exec(DynArray_T cmds,char* programName, char debug);

#endif