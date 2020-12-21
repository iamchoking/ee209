#include <stdio.h>
#include <unistd.h>

int main(){
	printf("sleepy: going to sleep for 1 sec\n");
	fflush(stdout);
	sleep(1);
	printf("sleepy: woken up\n");
	fflush(stdout);
}