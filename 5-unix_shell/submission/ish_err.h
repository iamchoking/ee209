/*--------------------------------------------------------------------*/
/* ish_err.h                                                          */
/* Author: 20190673 Hyungho Choi                                      */
/* 2020-2 EE209 Assignment 5                                          */
/*--------------------------------------------------------------------*/

#ifndef ISH_ERR_h
#define ISH_ERR_h

#define LINE_SIZE 1024

#include "dynarray.h"

DynArray_T aliases; //global variable. for checking aliases

char* PN; //program name for external circulation

enum{FAIL,SUCCESS};
enum{FALSE,TRUE};

// errno handling
void errno_handle(char* name,char debug);

// memory allocation
void err_alloc(char* programName);

//excecution - related///////////////////////
void err_cd(char* programName);
void err_setenv(char* programName);
void err_unsetenv(char* programName);
void err_exit(char* programName);
void err_alias(char* programName);
void err_pipe(char* programName);

//syntactic - related/////////////////////////
void err_destnotspec    (char* programName);
void err_noname         (char* programName);
void err_mult_redir_in  (char* programName);
void err_mult_redir_out (char* programName);
void err_input_noname   (char* programName);
void err_output_noname  (char* programName);

//parameter number errors for builtins
void err_params_cd      (char* programName);
void err_params_setenv  (char* programName);
void err_params_unsetenv(char* programName);
void err_params_exit    (char* programName);
void err_params_alias   (char* programName);

//lexical - related ///////////////////////////
void err_unmatched(char* programName);

//alias - related /////////////////////////////
void err_alias_not_found(char* key, char* programName);


#endif