/*--------------------------------------------------------------------*/
/* sig.h                                                              */
/* Author: 20190673 Hyungho Choi                                      */
/* 2020-2 EE209 Assignment 5                                          */
/*--------------------------------------------------------------------*/

#ifndef SIG_h
#define SIG_h

//this flag is set to 1 on the way to exit.
int ISH_EXIT_FLAG;

int install_sig_handler_shell(int debug);
int install_sig_handler_child(int debug);

#endif