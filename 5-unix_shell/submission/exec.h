/*--------------------------------------------------------------------*/
/* exec.h                                                             */
/* Author: 20190673 Hyungho Choi                                      */
/* 2020-2 EE209 Assignment 5                                          */
/*--------------------------------------------------------------------*/

#ifndef EXEC_h
#define EXEC_h

#include "dynarray.h"
#include "lex.h"
#include "syn.h"

int exec(DynArray_T cmds,char* programName, char debug);

#endif