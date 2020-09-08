/*wc209 by Hyungho Choi, KAIST 20190673*/
#include<stdio.h>
#include<ctype.h>

int curC; /*current character*/
int nxtC; /*next character*/
int befC; /*previous character*/
short state; /*state marker: 1: in script, 2: in comment (3:special case: only invoked right after transition to state 2)*/
int cmtl; /*last comment line. Used for error message*/

int cn; /*character number counter*/
int wn; /*word number counter */
int ln; /*oh you get the point*/

void progress(){ /*progresses all the characters.*/
    befC = curC;
    curC = nxtC;
    /*putchar(curC);
    printf("%d\n",state);*/
    if(curC == EOF){ /*reached the end of document. do not update nxtC*/
        return;
    }
    nxtC = getc(stdin);
    return;
}

void inscript(){ /*this is the code for when the code is in state 1 (in script)*/
    cn += 1;
    if (isspace(befC) && isspace(curC) == 0 && curC != EOF ){
        wn += 1;
        /*putchar('<');
        putchar(curC);
        putchar('>');
        putchar('\n');*/
    }
    if (curC == '\n'){
        ln += 1;
    }
}

void incomment(){
    if (curC == '\n'){
        ln += 1;
        cn += 1;
    }
    if(state == 3){ /*this is a workaround for the string [ / * / ] (without spaces)*/
        state = 2;
    }
}

int main(){
    if(curC == '\n' && nxtC == EOF){
        printf("0 0 0\n");
    }

    state = 1; /*initializing each variables*/
    nxtC = getc(stdin); /*used getc instead of getchar to easily change streams of input*/
    curC = '\n';
    cn = -1; /*since we basically start at position "negative 1," (with curC value set as '\n'), this workaround was chosen for init.*/
    wn = 0;
    ln = 0;
    cmtl = 0;
    

    while(curC != EOF){ /*the "state loop" begins*/
        if(state == 1){
            if(curC == '/' && nxtC == '*'){ /*state change condition*/
                /*putchar('<');
                putchar(curC);
                putchar('>');
                putchar('\n');*/
                state = 3; /*this is so that the configuration /*/  /*does not throw us off*/
                cmtl = ln;
            }
            else{
                inscript();
            }
        }

        else{
            if(curC == '*' && nxtC == '/' && state == 2){ /*state change condition*/
                state  =  1 ;
                nxtC   = ' '; /*this does the equivalent effect of treating the whole comment as a single space.*/
                /*putchar('[');
                putchar(curC);
                putchar(']');
                putchar('\n');*/
            }
            else{
                incomment();
            }
        }

        progress(); /*increments to next character*/
    }

    if(state == 2){ /*EOF was reached while the comment didn't finish -> error!*/
        fprintf(stderr,"Error: line %d: unterminated comment\n",cmtl);
        return 0;
    }
    printf("%d %d %d\n",ln,wn,cn);
    return 0;
}
