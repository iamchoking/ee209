#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "dynarray.h"
#include "lex.h"
#include "syn.h"
#include "exec.h"
#include "sig.h"

#define DIR_SIZE 1024 //defined for debug

// #define LINE_SIZE 1024 //defined in lex.h

char *programName;

DynArray_T tokens;
DynArray_T cmds;

enum{FAIL,SUCCESS};

int process(char* line,char debug){
	//processes 1 line the heart of everyting.
	// return 0;

	tokens = DynArray_new(0);
	cmds = DynArray_new(0);
	int suc = 0;

	if (!tokens || !cmds){
		fprintf(stderr, "%s: Cannot allocate memory\n", programName);
		exit(EXIT_FAILURE);
	}
	if(debug){
	printf("=============================\n");
	printf("-------------LEX-------------\n");
	}
	suc = lex_line(line, tokens,programName);
	if(debug){
		if (suc) {
			printf("<ISH> retrieved tokens:\n");
			print_tokens(tokens,1);
		}
		else{printf("<ISH> lex FAIL \n");}
	}
	if(!suc){goto process_end;}
	if(debug){printf("-------------SYN-------------\n");}
	suc = syn(tokens,cmds,programName);
	if(debug){
		if (suc){
			printf("<ISH> retrieved commands:\n");
			print_cmds(cmds);
		}
		else{printf("<ISH> syn FAIL \n");}
	}
	if(!suc){goto process_end;}
	// print_tokens(tokens,1);
	if(debug){printf("-------------EXEC------------\n");}
	suc = exec(cmds,programName,debug);
	// return 0;
	if(!suc && debug){printf("<ISH> exec FAIL \n");}
	if(debug){printf("=============================\n");}

	process_end:
		delete_cmds(cmds);
		delete_tokens(tokens);
	return suc;
}

int main(int argc, char *argv[]){


	programName = argv[0];
	char debug = 0;
	if(argc == 2 && !strcmp("-d",argv[1])){
		debug = 1;
		printf("%s: executed with debug options [<...>], pid: %d\n",programName,getpid());
	} //run with -d for debug output

	ISH_EXIT_FLAG = 0;
	install_sig_handler_shell(debug);
	// printf("Signal handlers installed\n");

	char cwd[DIR_SIZE];
	char line[LINE_SIZE];

	////////////////// processing ./ishrc //////////////////
	char dir_ishrc[LINE_SIZE];
	strcpy(dir_ishrc, getenv("HOME"));
	strcat(dir_ishrc,"/.ishrc");
	FILE * file_ishrc = fopen(dir_ishrc,"r");
	char* x;
	if(debug){printf("<ISH> Pharsing HOME/.ishrc file: ");}
	while((x=fgets(line, LINE_SIZE, file_ishrc)) != NULL){
		if(debug){printf("%d|%s ",getpid(),dir_ishrc);}
		printf("%% %s",line);
		process(line,debug);
		if(ISH_EXIT_FLAG) goto clean_exit;
	} //process finished.
	fclose(file_ishrc);
	if(debug){printf("<ISH> Pharsing Complete\n");}
	// free(file_ishrc);
	/////////////////////////////////////////////////////////

	// goto clean_exit;
	if(debug){printf("<ISH> Shell Commandline input mode\n");}
	if(debug){getcwd(cwd,DIR_SIZE);printf("%d|%s ",getpid(),cwd);}
	printf("%% ");

	while (fgets(line, LINE_SIZE, stdin)){
		if(line[0] == EOF){goto clean_exit;}
		process(line,debug);
		if(ISH_EXIT_FLAG) goto clean_exit;
		if(debug){getcwd(cwd,DIR_SIZE);printf("%d|%s ",getpid(),cwd);}
		printf("%% "); 
	}

	clean_exit:
		if(debug){printf("<ISH> clean exit");}
		printf("\n");
	return EXIT_SUCCESS; 
}