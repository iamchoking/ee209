/*--------------------------------------------------------------------*/
/* alias.c                                                            */
/* Author: 20190673 Hyungho Choi                                      */
/* 2020-2 EE209 Assignment 5                                          */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ish_err.h"
#include "alias.h"
#include "dynarray.h"
#include "syn.h"
#include "lex.h"

struct alias {
	char* key;
	char* value;
};

alias_t new_alias(char* key, char* value){
	assert(key&&value);

	alias_t alias = (alias_t)malloc(sizeof(struct alias));
	alias->key = (char*)calloc(strlen(key),sizeof(char));
	if(alias->key==NULL){err_alloc(PN);return NULL;}
	
	alias->value = (char*)calloc(strlen(value),sizeof(char));
	if(alias->key==NULL){err_alloc(PN);return NULL;}

	strcpy(alias->key,key);
	strcpy(alias->value,value);
	return alias;
}

char* alias_key(alias_t alias)   {assert(alias);return alias->key;}
char* alias_value(alias_t alias) {assert(alias);return alias->value;}

void free_alias(void* palias, void* dummy){
	assert(palias);

	alias_t alias = (alias_t)palias;
	free(alias->key);
	free(alias->value);
	free(alias);
}

void free_aliases(DynArray_T aliases){
	assert(aliases);

	DynArray_map(aliases,free_alias,NULL);
	DynArray_free(aliases);
}

void print_alias(void* palias, void* dummy){
	assert(palias);

	alias_t alias = (alias_t) palias;
	printf("alias %s='%s'\n",alias->key,alias->value);
}

void print_aliases(DynArray_T aliases){
	assert(aliases);
	DynArray_map(aliases, print_alias, NULL);
}

alias_t pharse_alias(char* arg){
	assert(arg);

	char temp_key[LINE_SIZE] = {0};
	char temp_value[LINE_SIZE] = {0};

	char* cur_key = temp_key;
	char* cur_value = temp_value;

	char* cur_arg = arg;

	char flag = 0; //flag 1 means we are pharsing the value.
	//this part is shaky... needs clarification.
	while(*cur_arg){
		// printf("pharsing alias: %c\n",*cur_arg);
		switch(*cur_arg){
			case '=':
				if(!flag) {flag = 1; cur_arg++;}
				else      {*(cur_value++)=*(cur_arg++);}
				break;
			default:
				if(!flag) {*(cur_key++)=*(cur_arg++);  }
				else      {*(cur_value++)=*(cur_arg++);}
				break;
		}
	}
	// if(!flag){return NULL;}
	// if cur_key is empty: error
	// if odd quotes, error
	*cur_key = '\0';
	*cur_value = '\0';
	return new_alias(temp_key,temp_value);
}

int compare_alias(const void* palias,const void* ppossible_key){
	assert(palias&&ppossible_key);

	alias_t alias = (alias_t)palias;
	char* possible_key = (char*) ppossible_key;
	return strcmp(alias->key,possible_key);
}


//returns corresponding index if aliases contains key. -1 if not.
int alias_search(DynArray_T aliases, char* key){
	assert(aliases&&key);

	return DynArray_search(aliases,key,compare_alias);
}

int add_alias(char* arg, DynArray_T aliases,int debug){
	assert(arg&&aliases);

	alias_t alias = pharse_alias(arg);
	int index;
	if(debug){
		printf("<ALIAS> add_alias: key: %s, value: %s\n",alias_key(alias),alias_value(alias));
		// int i = 0;
		// scanf("%d",&i);
	}
	if((index = alias_search(aliases,alias->key)) == -1){
		DynArray_add(aliases,alias);
		if(debug){printf("<ALIAS> alias successfully added\n");}
		return SUCCESS;
	}
	else{
		alias_t old_alias = DynArray_set(aliases,index,alias);
		free_alias(old_alias,NULL);
		if(debug){printf("<ALIAS> alias successfully updated\n");}
		return SUCCESS;
	}

} //run in exec_builtin (after syn)

int search_or_assign(char* arg){ //0:search, 1:assign
	assert(arg);

	char* cur = arg;
	if(*cur == '='){return 0;} //first letter =: auto search
	while(*cur){
		if(*cur == '='){return 1;}
		cur++;
	}
	return 0;
}

int handle_alias_function(cmd_t cmd,DynArray_T aliases, char*programName, int debug){
	assert(cmd&&aliases&&programName);

	if(cmd_len(cmd) == 2){
		char* arg = cmd_argv(cmd)[1];
		if(search_or_assign(arg)){
			//assign
			// printf("adding alias\n");
			if(!add_alias(arg,aliases,debug)){return FAIL;}
		}
		else{
			//search
			int result = alias_search(aliases,arg);
			if(result < 0){err_alias_not_found(arg,programName);return FAIL;}
			print_alias(DynArray_get(aliases,result),NULL);
		}
	}
	else{print_aliases(aliases);}
	return SUCCESS;
}

int stitchAt(DynArray_T stitchme, DynArray_T stitchthis, int stitchhere){
	assert(stitchme && stitchthis);

	for(int i = DynArray_getLength(stitchthis)-1;i>=0;i--){
		if(!DynArray_addAt(stitchme,stitchhere,DynArray_get(stitchthis,i))){
			return FAIL;
		}
	}
	DynArray_free(stitchthis);
	return SUCCESS;
}

int alias_preprocess(DynArray_T tokens, DynArray_T aliases, char* programName,int debug){
	assert(tokens && aliases && programName);

	//traverse backwards to stitching does not mess up the indices
	int alias_index;
	DynArray_T alias_tokens;
	alias_t alias;
	int hit_flag = 0;
	for(int token_index = DynArray_getLength(tokens)-1;token_index>=0;token_index--){
		token_t token = (token_t)DynArray_get(tokens,token_index);
		if(( alias_index = alias_search(aliases,token_value(token)) ) != -1){
			hit_flag = 1;
			if(debug){printf("<ALIAS> alias hit! token index: %d alias index: %d\n",token_index,alias_index);}
			alias = DynArray_get(aliases,alias_index);
			alias_tokens = DynArray_new(0);
			if(! lex_line(alias_value(alias),alias_tokens,TRUE,programName,debug)){return FAIL;}
			// if(debug){printf("<ALIAS> alias translated to: \n");
			// 	print_tokens(alias_tokens,TRUE);
			// }
			token_t old_token = DynArray_removeAt(tokens,token_index);
			delete_token(old_token);
			if( !stitchAt(tokens,alias_tokens,token_index)){return FAIL;}
		}
	}
	if(debug&&hit_flag){
		printf("<ALIAS> tokens update complete: \n");
		print_tokens(tokens,TRUE);
	}
	return SUCCESS;
}