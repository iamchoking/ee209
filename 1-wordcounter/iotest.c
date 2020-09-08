#include <stdio.h>

int main(){
    int curC;
    curC = getc(stdin);

    while (curC != EOF) {
        if(curC == '\n'){
            fputs("newline",stdout);
        }
        else if(curC == ' '){
            fputs("spc",stdout);
        }
        else{
            putchar(curC);
        }

        printf(" was detected \n");

        curC = getc(stdin);
    }    
    printf("EOF detected. Closing. \n\n");
    fprintf(stderr,"error just in case");
    return 0;
}
