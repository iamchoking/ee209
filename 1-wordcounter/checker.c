#include<stdio.h>

int main(int argc, char *argv[]){
    /*printf("Comparing files: %s and %s\n\n",argv[1],argv[2]);*/
    FILE *file1 = fopen(argv[1],"r");
    FILE *file2 = fopen(argv[2],"r");
    if(file1 == NULL || file2 == NULL){
        printf("FILE NOT FOUND\n\n");
        return 0;
    }

    int c1;
    int c2;
    while(1){
        c1 = fgetc(file1);
        c2 = fgetc(file2);
        printf("%c,%c",c1,c2);
        if (c1 != c2){
            printf("FAILED!!!!!!%c and %c \n",c1,c2);
            fclose(file1);
            fclose(file2);
            return 0;
        }
        if (getc(file1) == EOF){
            printf("PASSED\n");
            fclose(file1);
            fclose(file2);
            return 0;
        }
    }
}