#ifndef REDIR_h
#define REDIR_h

int redirect_in_check(cmd_t cmd);
int redirect_out_check(cmd_t cmd);

int redirect_in(cmd_t cmd,int index,char* programName,int debug);
int redirect_out(cmd_t cmd,int index,char* programName,int debug);

#endif