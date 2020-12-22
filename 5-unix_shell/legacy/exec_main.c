#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "dynarray.h"
#include "lex.h"
#include "syn.h"
#include "exec.h"

#define DIR_SIZE 1024

#define LINE_SIZE 1024

char *programName;

int main(int argc, char *argv[]){

	char cwd[DIR_SIZE];

	char line[LINE_SIZE];
	DynArray_T tokens;
	DynArray_T cmds;
	int suc;
	programName = argv[0];
	getcwd(cwd,DIR_SIZE);
	printf("%s%% ",cwd);
	while (fgets(line, LINE_SIZE, stdin)){
		tokens = DynArray_new(0);
		cmds = DynArray_new(0);

		if (!tokens || !cmds){
			fprintf(stderr, "%s: Cannot allocate memory\n", programName);
			exit(EXIT_FAILURE);
		}
		printf("=============================\n");
		printf("-------------LEX-------------\n");
		suc = lex_line(line, tokens,programName);
		if (suc) print_tokens(tokens,1);
		else{printf(">> lex FAILed \n");}

		printf("-------------SYN-------------\n");
		suc = syn(tokens,cmds,programName);
		if (suc) print_cmds(cmds);
		else{printf(">> syn FAILed \n");}
		// print_tokens(tokens,1);
		printf("-------------EXEC------------\n");
		suc = exec(cmds,programName);
		if(!suc){printf(">> exec FAILed \n");}
		printf("=============================\n");
		delete_cmds(cmds);
		delete_tokens(tokens);

		getcwd(cwd,DIR_SIZE);
		printf("%s%% ",cwd);
	}	

	return 0; 
}