#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANDOM_MAX 1024

int main() {
     srand(time(NULL)); // call srand at the beginning of the main
     int random_number = rand() % RANDOM_MAX;
	 printf("%d\n",random_number);
}