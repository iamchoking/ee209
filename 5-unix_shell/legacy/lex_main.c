#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "dynarray.h"
#include "lex.h"

#define LINE_SIZE 1024

char *programName;

int main(int argc, char *argv[]){
	char line[LINE_SIZE];
	DynArray_T tokens;
	int suc;
	programName = argv[0];

	while (fgets(line, LINE_SIZE, stdin)){
		tokens = DynArray_new(0);
	if (tokens == NULL){
	fprintf(stderr, "%s: Cannot allocate memory\n", programName);
	exit(EXIT_FAILURE);
	}
		suc = lex_line(line, tokens,programName);
		if (suc) print_tokens(tokens,1);
		delete_tokens(tokens);
	}	

	return 0; 
}