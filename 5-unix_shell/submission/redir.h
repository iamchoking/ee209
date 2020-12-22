/*--------------------------------------------------------------------*/
/* redir.h                                                            */
/* Author: 20190673 Hyungho Choi                                      */
/* 2020-2 EE209 Assignment 5                                          */
/*--------------------------------------------------------------------*/

#ifndef REDIR_h
#define REDIR_h

int redirect_in_check(cmd_t cmd);
int redirect_out_check(cmd_t cmd);

int redirect_in(cmd_t cmd,int index,char* programName,int debug);
int redirect_out(cmd_t cmd,int index,char* programName,int debug);

#endif