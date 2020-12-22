#ifndef ALIAS_h
#define ALIAS_h
#include "syn.h"

typedef struct alias * alias_t;

int handle_alias_function(cmd_t cmd,DynArray_T aliases, char*programName, int debug);
void free_aliases(DynArray_T aliases);

int alias_preprocess(DynArray_T tokens, DynArray_T aliases,char*programName,int debug);


#endif