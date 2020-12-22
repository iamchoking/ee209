#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dynarray.h"
#include "syn.h"
#include "lex.h"

#define NUM_BUILTINS 5
const char* ISH_BUILTINS[] = {"cd","setenv","unsetenv","exit","alias"};
//add alias here later

enum{FAIL,SUCCESS};
enum{FALSE,TRUE};



void err_destnotspec    (char* programName){fprintf(stderr, "%s: Pipe or redirection destination not specified\n",programName);}
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

struct cmd{
	enum cmdType type;
	int len;
	char *name;
	char **argv;
};

char* cmd_name(cmd_t cmd){return cmd->name;}
char** cmd_argv(cmd_t cmd){return cmd->argv;}
int cmd_len(cmd_t cmd){return cmd->len;}
int cmd_type(cmd_t cmd){return cmd->type;}

int checkif_builtin(char* name){
	// printf("%lu",sizeof(ISH_BUILTINS));
	for(int i = 0; i<NUM_BUILTINS; i++){
		if(!strcmp(ISH_BUILTINS[i],name)){return TRUE;}
		// printf("builtin %d: %s\n",i,ISH_BUILTINS[i]);
	}
	return FALSE;
}

void add_cmd(DynArray_T tokens, DynArray_T cmds, int startI, int endI){
	//adds from startI to endI (both inclusive)
	char* tmp; //tmporary for storing string value
	cmd_t cmd;
	cmd = (cmd_t)malloc(sizeof(struct cmd));
	cmd->len = endI-startI + 1;
	cmd->argv = calloc(cmd->len+1,sizeof(char*));
	for(int i = startI;i<=endI;i++){
		// tmp = "X";
		tmp = token_value((token_t)DynArray_get(tokens,i));
		cmd->argv[i-startI] = calloc(strlen(tmp),sizeof(char));
		strcpy(cmd->argv[i-startI],tmp);
		// printf("set %s to %s, result %d\n",tmp,cmd->argv[i],res);
	}
	cmd->argv[cmd->len] = NULL; //null termination of argv is important!
	cmd->name = cmd->argv[0];

	if(startI == 0 && checkif_builtin(cmd->name)){cmd->type = CMD_BUILTIN;}
	else{cmd->type = CMD_NORMAL;}

	DynArray_add(cmds,cmd);
}

int builtin_validate(cmd_t cmd,int total_len,char* progName){
	if(cmd->type != CMD_BUILTIN){return TRUE;}
	switch(cmd->name[0]){
		case 'c': //cd
			if((total_len != 1) || (cmd->len != 2 && cmd->len != 1) ){err_params_cd(progName);return FALSE;}
			break;
		case 's': //setenv
			if((total_len != 1) || (cmd->len != 3 && cmd->len != 2)){err_params_setenv(progName);return FALSE;}
			break;
		case 'u': //unsetenv
			if((total_len != 1) || (cmd->len != 2)){err_params_unsetenv(progName);return FALSE;}
			break;
		case 'e': //exit
			if((total_len != 1) || (cmd->len != 1)){err_params_exit(progName);return FALSE;}
			break;
		case 'a': //later for alias
			if((total_len != 1) || (cmd->len != 2 && cmd->len != 1)){err_params_alias(progName);return FALSE;}
			break;
	}
	return TRUE;
}

void free_cmd(void* pcmd, void* dummy){
	cmd_t cmd = (cmd_t)pcmd;
	for(int i = 0;i<cmd->len;i++){free (cmd->argv[i]);}
	free(cmd->argv);
	free(cmd);
}

void print_cmd(void* pcmd, void* dummy){
	cmd_t cmd = (cmd_t) pcmd;
	printf("<SYN> [ type: ");
	if(cmd->type == CMD_BUILTIN){printf("BUILTIN");}
	else{printf("NORMAL ");}
	printf(", len: %d, name: %s, args: {",cmd->len,cmd->name);
	for(int i=0;i<cmd->len;i++){printf("%s, ",cmd->argv[i]);}
	printf("} ]\n");
}

void print_cmds(DynArray_T cmds){
	DynArray_map(cmds, print_cmd, NULL);
	// cmd_t X = (DynArray_get(cmds,2),(void*)0);
	// printf("%s\n",X->name);
}

void delete_cmds(DynArray_T cmds){
	DynArray_map(cmds, free_cmd, NULL);
	DynArray_free(cmds);
}

enum REDIR_FLAG_INDEX{INDEX_PIPE,INDEX_REDIR_IN,INDEX_REDIR_OUT};

int check_flags(int type,int* redir_flags,char*programName){
	if(type == NORMAL){return SUCCESS;}
	// printf("%d,%d,%d",redir_flags[0],redir_flags[1],redir_flags[2]);
	//check flag phase
	if(type == PIPE && redir_flags[INDEX_PIPE]){err_mult_redir_out(programName);}
	else if(type == REDIR_OUT && redir_flags[INDEX_REDIR_OUT]){err_mult_redir_out(programName);}
	else if(type == REDIR_IN && redir_flags[INDEX_REDIR_IN]){err_mult_redir_in(programName);}
	else{
		//raise flag phase
		switch(type){
			case PIPE:
				redir_flags[REDIR_IN] = 1;
				break;
			case REDIR_IN:
				redir_flags[REDIR_IN] = 1;
				break;
			case REDIR_OUT:
				redir_flags[INDEX_PIPE] = 1;
				redir_flags[REDIR_OUT] = 1;
				break;		
		}
		return SUCCESS;
	}
	//flow reaches here if one of the errors were triggerd.
	return FAIL;
}

//fill up cmds while pharsing through tokens
int syn(DynArray_T tokens, DynArray_T cmds, char* programName){
	assert(tokens);
	enum State {START, OK, REQUIRED};
	enum State state = START;

	int current_start = 0; //starting index of current cmd
	token_t current;

	int len = DynArray_getLength(tokens);

	int redir_flags[3] = {0,0,0};
	//dfa method used
	for(int cur = 0; cur <= len ; cur++){ //dfa also checks for end condition, so cur increments 1 more than tokens length.
		if(cur<len){current = (token_t)DynArray_get(tokens,cur);} //the only place for retrieval of token
		switch (state){
		case START:
			if     (token_type(current) != NORMAL){err_noname(programName);return FAIL;}
			else if(cur == len){return SUCCESS;}
			else {state = OK;}
			break;
		
		case OK:
			if(token_type(current) == PIPE || cur == len){
				add_cmd(tokens,cmds,current_start,cur-1);
				current_start = cur+1;
			}
			if(token_type(current) != NORMAL){
				if(check_flags(token_type(current),redir_flags,programName) == FAIL){return FAIL;}
				state = REQUIRED;
			}
			else if(cur == len){return SUCCESS;}
			else{state = OK;}
			break;
		case REQUIRED:
			if(token_type(current) != NORMAL || cur == len){
				switch (token_type((token_t)DynArray_get(tokens,cur-1))){
				case PIPE:
					err_destnotspec(programName);
					break;
				case REDIR_OUT:
					err_output_noname(programName);
					break;
				case REDIR_IN:
					err_input_noname(programName);
					break;
				}
				printf("b\n");
				return FAIL;
				}
			else{state = OK;}
			break;
		default:
			fprintf(stderr,"<SYN>type error token #%d",cur);
			break;
		}
	
	}
	if(state == REQUIRED){err_destnotspec(programName);return FAIL;}
	if(current_start<len){add_cmd(tokens,cmds,current_start,len-1);}
	//initial cmd built

	if(len != 0){ return builtin_validate(DynArray_get(cmds,0),DynArray_getLength(cmds),programName);}

	return SUCCESS;
}