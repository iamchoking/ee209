#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
	int interval = 1;
	int beats = 5;
	if(argc >= 2){interval = atoi(argv[1]);}
	if (argc >= 3){beats = atoi(argv[2]);}
	for(int i = 0;i<beats;i++){
		sleep(interval);
		printf("%d\n",i+1);
		fflush(stdout);
	}
}