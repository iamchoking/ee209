/*--------------------------------------------------------------------*/
/* lex.c                                                              */
/* Author: 20190673 Hyungho Choi                                      */
/* 2020-2 EE209 Assignment 5                                          */
/*--------------------------------------------------------------------*/

#include "dynarray.h"
#include "lex.h"
#include "ish_err.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


#define DEBUG_LEX FALSE //change this to show step by step

// const char* ISH_BUILTINS[4] = {"cd","setenv","unsetenv","exit"};
//add alias here later

struct Token {
	enum TokenType type;
	char* value;
};

int token_type(token_t token){return token->type;}
char* token_value(token_t token){return token->value;}

token_t new_token(enum TokenType type, char* value){
	assert(value);

	token_t token;
	token = (token_t)malloc(sizeof(struct Token));
	if(token == NULL){err_alloc(PN); return NULL;} //alloc failed

	token->type = type;
	token->value = (char*)malloc(strlen(value));
	if(token->value == NULL){err_alloc(PN); return NULL;} //alloc failed

	if(token->value == NULL) return NULL;

	strcpy(token->value,value);

	return token;
}


void delete_token(token_t token){
	assert(token);
	free(token->value);
	free(token);
}
void free_token(void* token, void* dummy){
	assert(token);
	delete_token((token_t)token);
}

void delete_tokens(DynArray_T tokens){
	assert(tokens);
	DynArray_map(tokens, free_token, NULL);
	DynArray_free(tokens);
	// free(tokens);
}

int isend(char c){ //checks if c is an end-like character
	if((c == '\n')||(c == '\0')||(c == EOF)){return 1;}
	return 0;
}

int print_token(token_t token, char include_type){// include_type = 0 for simple print, 1 for specifying token type
	assert(token);
	if(include_type) printf("<LEX> [");
	if(include_type){
		switch (token->type){
			case(PIPE):
			printf("PIPE     , ");
			break;

			case(NORMAL):
			printf("NORMAL   , ");
			break;

			case(REDIR_IN):
			printf("REDIR_IN , ");
			break;			

			case(REDIR_OUT):
			printf("REDIR_OUT, ");
			break;
		}
	}

	printf("%s",token->value);
	if(include_type) printf("]\n");
	else printf("\n");
	return token->type;
} 

void print_token_lame(void* token, void* dummy){
	assert(token);
	print_token((token_t)token,0);
}
void print_token_lame_with_type(void* token, void* dummy){
	assert(token);
	print_token((token_t)token,1);
}

void print_tokens(DynArray_T tokens, char include_type){
	assert(tokens);
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
	assert(tokens && programName);
	token_t token = new_token(type,value);
	if(!token){ err_alloc(programName); return FAIL;}
	if(!DynArray_add(tokens,token)){err_alloc(programName); return FAIL;}
	return SUCCESS;
}

int lex_alias(char** cur_line_ref,DynArray_T tokens,char* programName,int debug);
//declared here, defined later

int lex_line(char* line, DynArray_T tokens, int supress, char* programName,int debug){
	assert(line);
	assert(tokens);
	assert(programName);

	enum State{IDLE, IN_WORD, IN_QUOTE};

	enum State state = IDLE;
	char value_temp[LINE_SIZE];
	// enum TokenType cur_type = NORMAL;
	char* cur_val = value_temp; // cursor (traverses through temp. value)
	char* cur_line = line; //cursor (traverses through line)
	while(1){
		if(DEBUG_LEX){printf("Char: %c, State: %d\n",*cur_line,state);}
		
		switch (state){
		case IDLE:
			if (isspace(*cur_line)){
				cur_line++;
				break;
			}
			else if(isend(*cur_line)){return SUCCESS;}
			else if(*cur_line == '|'){
				if(supress){add_nicely(NORMAL,"|",tokens,programName);}
				else{add_nicely(PIPE,"|",tokens,programName);}
				cur_line++;
				break;
			}
			else if(*cur_line == '>'){
				if(supress){add_nicely(NORMAL,">",tokens,programName);}
				else{add_nicely(REDIR_OUT,">",tokens,programName);}
				cur_line++;
				break;
			}
			else if(*cur_line == '<'){
				if(supress){add_nicely(NORMAL,"<",tokens,programName);}
				else{add_nicely(REDIR_IN,"<",tokens,programName);}
				cur_line++;
				break;
			}
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
			if(isspace(*cur_line)||isend(*cur_line)||(*cur_line == '|')||(*cur_line == '<')||(*cur_line == '>') ){ //add < and > here later
				*(cur_val) = '\0';
				// if(check_builtin2(value_temp)){
				// 	add_nicely(BUILTIN,value_temp,tokens,programName);
				// }
				// add_nicely(NORMAL,value_temp,tokens,programName);
				// DynArray_add(tokens,new_token(NORMAL,value_temp));
				add_nicely(NORMAL,value_temp,tokens,programName);
				if(! strcmp(value_temp,"alias")){
					if(debug){printf("<LEX> alias detected! \n");}
					if(lex_alias(&cur_line,tokens,programName,debug)==FAIL){return FAIL;}
				}
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
				// printf("%c",*cur_val);
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

//lexical analysis of alias argument.
int lex_alias(char** cur_line_ref,DynArray_T tokens,char* programName,int debug){
	assert(cur_line_ref && tokens && programName);

	char* cur_alias_reader = *cur_line_ref;
	char alias_temp[LINE_SIZE] = {0};
	char* cur_alias_writer = alias_temp;

	enum quoteIndex{IND_SQ,IND_DQ};
	int quote_flags[2] = {0,0};
	while(isspace(*cur_alias_reader)){
		cur_alias_reader++;
	}
	while(TRUE){
		// printf("%c",*cur_alias_reader);
		if(isspace(*cur_alias_reader) || isend(*cur_alias_reader || *cur_alias_reader == '|'||*cur_alias_reader == '<'||*cur_alias_reader == '>')){
			if(quote_flags[IND_SQ]==0 && quote_flags[IND_DQ]==0){
				//add
				*cur_alias_writer = '\0';
				if(cur_alias_writer == alias_temp){return SUCCESS;}
				add_nicely(NORMAL,alias_temp,tokens,programName);
				*cur_line_ref = cur_alias_reader;
				if(debug){printf("<LEX> captured alias: %s\n",alias_temp);}
				return SUCCESS;
			}
			else if (isspace(*cur_alias_reader)){
				*(cur_alias_writer++) = *(cur_alias_reader++);
				}
			else{err_unmatched(programName);return FAIL;}
		}
		else if (*cur_alias_reader == '\''){
			if (quote_flags[IND_DQ] == 0){
				if(quote_flags[IND_SQ]){quote_flags[IND_SQ] = 0;}
				else{quote_flags[IND_SQ] = 1;}
				cur_alias_reader++;
			}
			else{*(cur_alias_writer++) = *(cur_alias_reader++);}
		}
		else if (*cur_alias_reader == '"'){
			if(quote_flags[IND_DQ]){quote_flags[IND_DQ] = 0;}
			else{quote_flags[IND_DQ] = 1;}
			cur_alias_reader++;
		}
		else{*(cur_alias_writer++) = *(cur_alias_reader++);}
	}
	return FAIL;
}