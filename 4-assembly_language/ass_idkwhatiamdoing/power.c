#include <stdio.h>

static int iBase, iExp, iIndex, iPower = 1; // first 3 in BSS, iPower in Data.

int main(void){
	scanf("%d", &iBase);
	scanf("%d", &iExp);

	//computing with for loop
	for (iIndex = 1; iIndex <= iExp; iIndex++){ iPower *= iBase;}

	printf("%d to the power of %d is %d.\n", iBase, iExp, iPower);
	return 0;
}

//excercise in precept 7 is to convert this to ass language