#include "dynarray.h"
#include "syn.h"
#include "sig.h"
#include "redir.h"
#include "ish_err.h"
#include "exec.h"
#include "alias.h"

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<errno.h>

int exec_builtin(cmd_t cmd, char* programName,char debug){
	if(cmd_type(cmd) != CMD_BUILTIN){return SUCCESS;}
	//all validation checks are done in syntactic step.
	int len = cmd_len(cmd);
	char* name = cmd_name(cmd);
	char** argv = cmd_argv(cmd);
	if(debug){printf("<EXEC> BUILTIN: %s triggered\n",name);}
	switch(name[0]){
		case 'c': //cd
			// printf("cd triggered\n");
			if(len == 1){chdir(getenv("HOME"));}
			else if(chdir(argv[1]) == -1){err_cd(programName);return FAIL;}
			break;
		case 's': //setenv
			// printf("setenv triggered\n");
			if(len == 2){setenv(argv[1],"",1);}
			else if(setenv(argv[1],argv[2],1)==-1){err_setenv(programName);return FAIL;}
			break;
		case 'u': //unsetenv
			// printf("unsetenv triggered\n");
			if(unsetenv(argv[1])==-1){err_unsetenv(programName);return FAIL;}
			break;
		case 'e': //exit
			// printf("exit triggered\n");
			ISH_EXIT_FLAG = 1;
			break;
		case 'a': //later for alias
			handle_alias_function(cmd,aliases,programName,debug);
			break;
	}
	return SUCCESS;
}

int exec(DynArray_T cmds,char* programName,char debug){
	int cmd_len_total = DynArray_getLength(cmds);
	// cmd_t cur_cmd; //cursor for current cmd
	if(cmd_len_total == 0) return SUCCESS;
	// take care of first cmd which is the only potential builtin.
	if(cmd_type(DynArray_get(cmds,0)) == CMD_BUILTIN){
		if(! exec_builtin(DynArray_get(cmds,0),programName,debug)){return FAIL;}
		return SUCCESS;
	}

	//used for redirection.
	int redir_in_index ;
	int redir_out_index;

	int status = 0; // used for waiting / status checking
	int fin_pid; //used later in catching finished pid (debug purposes)

	//now, we are ready to fork. and ignore the CMD_BUILTIN commands
	fflush(stdin);fflush(stdout);fflush(stderr);
	int pid = fork(); //initial fork away from shell process
	if (pid == 0){ 
		for(int i =3;i<10;i++){close(i);} //IMPORTANT: close all but default file descriptors
		if(debug){printf("<EXEC> pipe root   process started : %d\n",getpid());}
		// exit(0);
		// first normal process in pipeline = parent of all further processes
		//uninstall installed signal handlers here.
		install_sig_handler_child(debug);
		// int status_2;
		int pid_2;
		cmd_t cmd_cur;
		int fd[2]; //file descriptors for pipe
		if(cmd_len_total > 0){
			for(int i = 0; i < cmd_len_total; i++){
				if(pipe(fd)==-1){err_pipe(programName);exit(EXIT_FAILURE);}
				pid_2 = fork();
				if(!pid_2){ //parent process: redirect stdout to fd[1] and run process
					if(debug){printf("<EXEC> pipe branch process started : %d\n",getpid());}
					if(i<cmd_len_total-1){
						dup2(fd[1],1);
						close(fd[0]);
						close(fd[1]);
						// fprintf(stderr,"stdout redirected: %d\n",i);
					}
					cmd_cur = (cmd_t)DynArray_get(cmds,i);
					//REDIRECTION: note that only the first command can redirect in / last command can redirect out
					if(i == 0               && (redir_in_index  = redirect_in_check (cmd_cur))){
						if(redirect_in (cmd_cur,redir_in_index,programName,debug) == FAIL){exit(EXIT_FAILURE);}
					}
					if(i == cmd_len_total-1 && (redir_out_index = redirect_out_check(cmd_cur))){
						if(redirect_out(cmd_cur,redir_out_index,programName,debug) == FAIL){exit(EXIT_FAILURE);}
					}
					// fprintf(stderr,"executing cmd # %d: %s\n",i,cmd_name(cmd_cur));
					execvp(cmd_name(cmd_cur),cmd_argv(cmd_cur));

					// char message[1024];
					// explain_message_execvp(message, sizeof(message), cmd_name(cmd_cur), cmd_argv(cmd_cur));
					// fprintf(stderr, "%s\n", message);
					// exit(EXIT_FAILURE);
					// int err = errno;//execvp failed
					// fprintf(stderr, "%s\n", 
					// explain_errno_execvp(err,cmd_name(cmd_cur), cmd_name(cmd_cur)));
					// exit(EXIT_FAILURE);

					errno_handle(cmd_name(cmd_cur),debug);
					exit(EXIT_FAILURE);

					// assert(0); 
				}
				else{ //child process: redirect stdin and await further processes (if this is last, no need.)
					if(i>=0){
						dup2(fd[0],0);
						close(fd[0]);
						close(fd[1]);
						// fprintf(stderr,"stdin redirected: %d\n",i);
					}
					//can't finish until parent (upstream process) finishes
				}
			}
			// wait(&status_2); 
			while((fin_pid = wait(&status))>0){
				fflush(stdin);fflush(stdout);fflush(stderr);
				if(debug){printf("<EXEC> pipe branch process finished: %d (exit status %d)\n",fin_pid,status);}
			}
		fflush(stdin);fflush(stdout);fflush(stderr);
		}
		exit(EXIT_SUCCESS);
	} //end of initial fork
	else{
		// shell waits for all process to finish
		while((fin_pid = wait(&status))>0){
			fflush(stdin);fflush(stdout);fflush(stderr);
			if(debug){printf("<EXEC> pipe root   process finished: %d (exit status %d)\n",fin_pid,status);}
		}
	}
	// while(wait(&status) > 0){}
	// waitpid(pid,&status,0); 
	return SUCCESS;
}