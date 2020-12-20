#include "dynarray.h"
#include "lex.h"

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>

#define LINE_SIZE 1024

#define DEBUG FALSE //change this to show step by step

enum {FALSE, TRUE};
enum {FAIL, SUCCESS};

// const char* ISH_BUILTINS[4] = {"cd","setenv","unsetenv","exit"};
//add alias here later

struct Token {
	enum TokenType type;
	char* value;
};

int token_type(token_t token){return token->type;}
char* token_value(token_t token){return token->value;}

token_t new_token(enum TokenType type, char* value){
	token_t token;
	token = (token_t)malloc(sizeof(struct Token));
	if(token == NULL) return NULL; //alloc failed

	token->type = type;
	token->value = (char*)malloc(strlen(value));
	if(token->value == NULL) return NULL;

	strcpy(token->value,value);

	return token;
}


void delete_token(token_t token){
	free(token->value);
	free(token);
}
void free_token(void* token, void* dummy){
	delete_token((token_t)token);
}

void delete_tokens(DynArray_T tokens){
	DynArray_map(tokens, free_token, NULL);
	DynArray_free(tokens);
}

int isend(char c){ //checks if c is an end-like character
	if((c == '\n')||(c == '\0')||(c == EOF)){return 1;}
	return 0;
}

void err_alloc(char* programName){fprintf(stderr, "%s: Memory Allocation Failed\n",programName);}
void err_unmatched(char* programName){fprintf(stderr, "%s: ERROR - unmatched quote\n",programName);}

int print_token(token_t token, char include_type){// include_type = 0 for simple print, 1 for specifying token type
	if(include_type) printf("[");
	if(include_type){
		switch (token->type){
			case(PIPE):
			printf("PIPE, ");
			break;

			case(NORMAL):
			printf("NORMAL, ");
			break;

			case(REDIR_IN):
			printf("REDIR_IN, ");
			break;			

			case(REDIR_OUT):
			printf("RDIR_OUT, ");
			break;
		}
	}

	printf("%s",token->value);
	if(include_type) printf("]\n");
	else printf("\n");
	return token->type;
} 

void print_token_lame(void* token, void* dummy){
	print_token((token_t)token,0);
}
void print_token_lame_with_type(void* token, void* dummy){
	print_token((token_t)token,1);
}

void print_tokens(DynArray_T tokens, char include_type){
	if(include_type){DynArray_map(tokens, print_token_lame_with_type, NULL);}
	else{DynArray_map(tokens, print_token_lame, NULL);}
}

// int check_builtin2(char* value_temp){
// 	// for(int i = 0; i<sizeof(ISH_BUILTINS); i++){
// 	// 	if(!strcmp(ISH_BUILTINS[i],value_temp));
// 	// 	return TRUE;
// 	// }
// 	return FALSE;
// }

int add_nicely(enum TokenType type, char* value, DynArray_T tokens, char* programName){
	token_t token = new_token(type,value);
	if(!token){ err_alloc(programName); return FAIL;}
	if(!DynArray_add(tokens,token)){err_alloc(programName); return FAIL;}
	return SUCCESS;
}

int lex_line(char* line, DynArray_T tokens, char* programName){
	assert(line && tokens);

	enum State{IDLE, IN_WORD, IN_QUOTE};

	enum State state = IDLE;
	char value_temp[LINE_SIZE];
	// enum TokenType cur_type = NORMAL;
	char* cur_val = value_temp; // cursor (traverses through temp. value)
	char* cur_line = line; //cursor (traverses through line)

	while(1){
		if(DEBUG){printf("Char: %c, State: %d\n",*cur_line,state);}
		switch (state){
		case IDLE:
			if (isspace(*cur_line)){
				cur_line++;
				break;
			}
			else if(isend(*cur_line)){return SUCCESS;}
			else if(*cur_line == '|'){
				add_nicely(PIPE,"|",tokens,programName);
				cur_line++;
				break;
			}
			// else if(*cur_line == ">"){} //currently omitted
			else if(*cur_line == '"'){
				state = IN_QUOTE;
				// cur_type = NORMAL;
				cur_line++;
				break;
			} //this part can actually be omitted, and cause no problems.
			else{ //should mean it is in a word
				// cur_type = NORMAL;
				state = IN_WORD;
				break; // no advance
			}
			break;

		case IN_WORD:
			if(isspace(*cur_line)||isend(*cur_line)||(*cur_line == '|')){ //add < and > here later
				*(cur_val) = '\0';
				// if(check_builtin2(value_temp)){
				// 	add_nicely(BUILTIN,value_temp,tokens,programName);
				// }
				add_nicely(NORMAL,value_temp,tokens,programName);
				// printf("Successfully added.\n");
				cur_val = value_temp;
				state = IDLE;
				break; //no advance
			}
			else if(*cur_line == '"'){
				state = IN_QUOTE;
				cur_line++;
				break;
			}
			else{
				*cur_val = *cur_line;
				cur_line++;
				cur_val++;
				break;
			}
			break;


		case IN_QUOTE:
			if(*cur_line == '"'){
				state = IN_WORD;
				cur_line++;
				break;
			}
			else if(isend(*cur_line)){err_unmatched(programName);return FAIL;}
			else{
				*cur_val = *cur_line;
				cur_line++;
				cur_val++;
				break;
			}
			break;
		default:
			break;  
		}
	}
}