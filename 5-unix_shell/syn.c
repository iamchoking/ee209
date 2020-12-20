#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dynarray.h"
#include "syn.h"
#include "lex.h"

#define NUM_BUILTINS 4
const char* ISH_BUILTINS[] = {"cd","setenv","unsetenv","exit"};
//add alias here later

enum{FAIL,SUCCESS};
enum{FALSE,TRUE};



void err_destnotspec(char* programName){fprintf(stderr, "%s: Pipe or redirection destination not specified\n",programName);}
void err_noname(char* programName){fprintf(stderr, "%s: Missing command name\n",programName);}

void err_builtin_cd(char* programName){fprintf(stderr, "%s: cd takes one parameter\n",programName);}
void err_builtin_setenv(char* programName){fprintf(stderr, "%s: setenv takes one or two parameters\n",programName);}
void err_builtin_unsetenv(char* programName){fprintf(stderr, "%s: unsetenv takes one parameter\n",programName);}
void err_builtin_exit(char* programName){fprintf(stderr, "%s: exit does not take any parameters\n",programName);}
void err_builtin_alias(char* programName){fprintf(stderr, "%s: alias takes one parameter\n",programName);}

struct cmd{
	enum cmdType type;
	int len;
	char *name;
	char **argv;
};

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
	cmd->argv = calloc(cmd->len,sizeof(char*));
	for(int i = startI;i<=endI;i++){
		// tmp = "X";
		tmp = token_value((token_t)DynArray_get(tokens,i));
		cmd->argv[i-startI] = calloc(strlen(tmp),sizeof(char));
		strcpy(cmd->argv[i-startI],tmp);
		// printf("set %s to %s, result %d\n",tmp,cmd->argv[i],res);
	}
	cmd->name = cmd->argv[0];

	if(startI == 0 && checkif_builtin(cmd->name)){cmd->type = CMD_BUILTIN;}
	else{cmd->type = CMD_NORMAL;}

	DynArray_add(cmds,cmd);
}

int builtin_validate(cmd_t cmd,char* progName){
	if(cmd->type != CMD_BUILTIN){return TRUE;}
	switch(cmd->name[0]){
		case 'c': //cd
			if(cmd->len != 2 && cmd->len != 1){err_builtin_cd(progName);return FALSE;}
			break;
		case 's': //setenv
			if(cmd->len != 3 && cmd->len != 2){err_builtin_setenv(progName);return FALSE;}
			break;
		case 'u': //unsetenv
			if(cmd->len != 2){err_builtin_unsetenv(progName);return FALSE;}
			break;
		case 'e': //exit
			if(cmd->len != 1){err_builtin_exit(progName);return FALSE;}
			break;
		case 'a': //later for alias
			if(cmd->len != 2){err_builtin_alias(progName);return FALSE;}
			break;
	}
	return TRUE;
}

void free_cmd(void* pcmd, void* dummy){
	cmd_t cmd = (cmd_t)pcmd;
	for(int i = 0;i<cmd->len;i++){free (cmd->argv[i]);}
	free(cmd);
}

void print_cmd(void* pcmd, void* dummy){
	cmd_t cmd = (cmd_t) pcmd;
	printf("<CMD> [ type: ");
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

//fill up cmds while pharsing through tokens
int syn(DynArray_T tokens, DynArray_T cmds, char* programName){
	assert(tokens);
	enum State {START, OK, REQUIRED};
	enum State state = START;

	int current_start = 0; //starting index of current cmd
	token_t current;

	int len = DynArray_getLength(tokens);

	for(int cur = 0; cur < len ; cur++){
		current = (token_t)DynArray_get(tokens,cur);
		switch (state){
		case START:
			if(token_type(current) != NORMAL){
				err_noname(programName);
				return FAIL;
			}
			else{
				// if((! strcmp(token_value(current),"cd")) && len > 2){
				// 	err_builtin_cd(programName);
				// } //poetntially do builtin error handling here
				state = OK;
			}
			break;
		
		case OK:
			if(token_type(current) == PIPE){
				// printf("c_s: %d, c:%d\n",current_start,cur);
				add_cmd(tokens,cmds,current_start,cur-1);
				current_start = cur+1;
			}
			if(token_type(current) != NORMAL){state = REQUIRED;}
			break;
		case REQUIRED:
			if(token_type(current) != NORMAL){err_destnotspec(programName);return FAIL;}
			else{state = OK;}
			break;
		default:
			break;
		}
	
	}
	if(state == REQUIRED){err_destnotspec(programName);return FAIL;}
	if(current_start<len){add_cmd(tokens,cmds,current_start,len-1);}
	//initial cmd built

	if(len != 0){ return builtin_validate(DynArray_get(cmds,0),programName);}

	return SUCCESS;
}