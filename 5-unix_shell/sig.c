#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "sig.h"
int ISH_SIGQUIT_FLAG = 0;

enum{FAIL,SUCCESS};

static void handler_SIGQUIT(int iSig){
	if(ISH_SIGQUIT_FLAG){
		fflush(stdout);
		exit(EXIT_SUCCESS);
		// ISH_EXIT_FLAG = 1;
	}
	else{
		printf("Type Ctrl-\\ again within 5 seconds to exit.\n");
		ISH_SIGQUIT_FLAG = 1;
		fflush(stdout);
		alarm(5);
	}
}

static void handler_SIGALRM(int iSig){
	ISH_SIGQUIT_FLAG = 0;
}

static void handler_SIGALRM_debug(int iSig){
	ISH_SIGQUIT_FLAG = 0;
	printf("\n<SIG> flag reset (5 seconds passed)\n");
}

int install_sig_handler_shell(int debug){
	sigset_t sigset_essential;
	sigemptyset(&sigset_essential);
	sigaddset(&sigset_essential, SIGINT);
	sigaddset(&sigset_essential, SIGQUIT);
	sigaddset(&sigset_essential, SIGALRM);

	sigprocmask(SIG_UNBLOCK, &sigset_essential , NULL);

	signal(SIGINT , SIG_IGN);
	signal(SIGQUIT, handler_SIGQUIT);
	if(debug){signal(SIGALRM, handler_SIGALRM_debug);}
	else{signal(SIGALRM, handler_SIGALRM);}

	if(debug){printf("<SIG> Signal Handlers Installed for: SHELL\n");}
	return SUCCESS;
} 

int install_sig_handler_child(int debug){
	signal(SIGINT , SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGALRM, SIG_DFL);

	if(debug){printf("<SIG> Signal Handlers Updated for: CHILD\n");}
	return SUCCESS;
}