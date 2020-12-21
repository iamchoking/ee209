#include <stdio.h>

int main(){
	int x;
	while(2){
		if(scanf("%d",&x) <= 0)return 0;
		printf("%d\n",x*2);
		fflush(stdout);
	}
	return 0;
}