#include "ish_err.h"

#include <stdio.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

void errno_handle(char* name,char debug){
	if(debug){fprintf(stderr,"<ERR> [<ERRNO ERR> pid: %d errno: %d]\n",getpid(),errno);}
	switch (errno){
	case 1 : fprintf(stderr,"%s: Operation not permitted\n",name); break;
	case 2 : fprintf(stderr,"%s: No such file or directory\n",name);	break;
	case 5 : fprintf(stderr,"%s: I/O error\n",name);	break;
	case 13: fprintf(stderr,"%s: Permission Denied\n",name);	break;
	case 14: fprintf(stderr,"%s: Bad address\n",name); break;
	case 26: fprintf(stderr,"%s: Text file busy\n",name); break;
	default:
		fprintf(stderr,"%s: prgram exited with errno %d\n",name,errno ); break;
	}
}

void err_cd(char* programName){fprintf(stderr, "%s: No such file or directory\n",programName);}
void err_setenv(char* programName){fprintf(stderr, "%s: setenv failed\n",programName);}
void err_unsetenv(char* programName){fprintf(stderr, "%s: unsetenv failed\n",programName);}
void err_exit(char* programName){fprintf(stderr, "%s: exit failed\n",programName);}
void err_alias(char* programName){fprintf(stderr, "%s: alias failed\n",programName);}

void err_pipe(char* programName){fprintf(stderr, "%s: pipe creation failed.",programName);}


void err_destnotspec    (char* programName){fprintf(stderr, "%s: Pipe or redirection destination is not specified\n",programName);}
void err_noname         (char* programName){fprintf(stderr, "%s: Missing command name\n",programName);}
// new errors from redirection
void err_mult_redir_in  (char* programName){fprintf(stderr, "%s: Multiple redirection of standard input\n",programName);}
void err_mult_redir_out (char* programName){fprintf(stderr, "%s: Multiple redirection of standard out\n",programName);}
void err_input_noname   (char* programName){fprintf(stderr, "%s: Standard input redirection without file name\n",programName);}
void err_output_noname  (char* programName){fprintf(stderr, "%s: Standard output redirection without file name\n",programName);}
/////

///parameter number errors for builtins
void err_params_cd      (char* programName){fprintf(stderr, "%s: cd takes one parameter\n",programName);}
void err_params_setenv  (char* programName){fprintf(stderr, "%s: setenv takes one or two parameters\n",programName);}
void err_params_unsetenv(char* programName){fprintf(stderr, "%s: unsetenv takes one parameter\n",programName);}
void err_params_exit    (char* programName){fprintf(stderr, "%s: exit does not take any parameters\n",programName);}
void err_params_alias   (char* programName){fprintf(stderr, "%s: alias takes one parameter\n",programName);}


void err_alloc(char* programName){fprintf(stderr, "%s: Memory Allocation Failed\n",programName);}
void err_unmatched(char* programName){fprintf(stderr, "%s: Could not find quote pair\n",programName);}

void err_alias_not_found(char* key, char* programName){fprintf(stderr, "%s: alias: %s: not found\n",programName,key);}
