#include <stdio.h>
// flattened version of power.c

static int iBase, iExp, iIndex, iPower = 1; // first 3 in BSS, iPower in Data.

int main(void){
	scanf("%d", &iBase);
	scanf("%d", &iExp);

	//computing without for loop (flattened)
	iIndex = 1;
	
	loop1:

	if (iIndex > iExp){
		goto loopend1;
	}

	iPower *= iBase;
	iIndex ++;
	goto loop1;

	loopend1:

	printf("%d to the power of %d is %d.\n", iBase, iExp, iPower);
	return 0;
}

//excercise in precept 7 is to convert this to ass language