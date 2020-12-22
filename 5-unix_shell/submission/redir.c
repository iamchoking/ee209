/*--------------------------------------------------------------------*/
/* redir.c                                                            */
/* Author: 20190673 Hyungho Choi                                      */
/* 2020-2 EE209 Assignment 5                                          */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include"syn.h"
#include"redir.h"
#include"ish_err.h"


int redirect_in_check(cmd_t cmd){
	assert(cmd);

	char ** argv = cmd_argv(cmd);
	for(int i = 0; i<cmd_len(cmd); i++){
		if(! strcmp("<",argv[i])){
			return i;
		}
	}
	return 0;
}

int redirect_out_check(cmd_t cmd){
	assert(cmd);

	char ** argv = cmd_argv(cmd);
	for(int i = 0; i<cmd_len(cmd); i++){
		if(! strcmp(">",argv[i])){
			return i;
		}
	}
	return 0;
}

int redirect_in(cmd_t cmd,int index,char* programName,int debug){
	assert(cmd && programName);

	char ** newargv = calloc(cmd_len(cmd)-1,sizeof(char*));
	if(newargv == NULL){err_alloc(programName);return FAIL;}

	char ** argv = cmd_argv(cmd);
	char *  target; //target for redirection

	if(debug){
		printf("<REDIR> REDIRECTION-IN detected: %s\n\t",cmd_name(cmd));
		print_cmd(cmd,NULL);
		printf("<REDIR> changed to:\n\t");
	}

	for(int i = 0; i<=cmd_len(cmd); i++){
		if(i<index)             {newargv[i] = argv[i];}
		else if(i == index)     {free(argv[i]);} //free the redirection operator itself
		else if(i == index + 1) {target = argv[i];} //syntactic analysis made sure that index + 1 is valid
		else                    {newargv[i-2] = argv[i];}
	}

	cmd_set_argv(cmd,newargv);
	cmd_set_len (cmd,cmd_len(cmd)-2);
	if(debug){print_cmd(cmd,NULL);}

	int in_fd = open(target, O_RDONLY); 
	free(target);
	if(in_fd == -1){errno_handle(programName,debug);return FAIL;}
	fflush(stdin);fflush(stdout);fflush(stderr);

	dup2(in_fd,0);
	close(in_fd);
	
	return SUCCESS;
}

int redirect_out(cmd_t cmd,int index,char* programName,int debug){
	assert(cmd && programName);

	char ** newargv = calloc(cmd_len(cmd)-1,sizeof(char*)); //total len - 2 + 1 due to NULL termination
	if(newargv == NULL){err_alloc(programName);return FAIL;}

	char ** argv = cmd_argv(cmd);
	char *  target; //target for redirection

	if(debug){
		printf("<REDIR> REDIRECTION-OUT detected: %s\n\t",cmd_name(cmd));
		print_cmd(cmd,NULL);
		printf("<REDIR> changed to:\n\t");
	}

	for(int i = 0; i<=cmd_len(cmd); i++){
		if(i<index)             {newargv[i] = argv[i];}
		else if(i == index)     {free(argv[i]);} //free the redirection operator itself
		else if(i == index + 1) {target = argv[i];} //syntactic analysis made sure that index + 1 is valid
		else                    {newargv[i-2] = argv[i];}
	}
	cmd_set_argv(cmd,newargv);
	cmd_set_len (cmd,cmd_len(cmd)-2);
	if(debug){print_cmd(cmd,NULL);}

	int out_fd = open(target,O_WRONLY|O_CREAT|O_TRUNC,0666);
	free(target);
	if(out_fd == -1){errno_handle(programName,debug);return FAIL;}
	fflush(stdin);fflush(stdout);fflush(stderr);

	dup2(out_fd,1);
	close(out_fd);

	return SUCCESS;
}
