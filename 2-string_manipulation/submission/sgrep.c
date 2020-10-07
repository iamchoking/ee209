/*20190673 Hyungho Choi Assignment 2 sgrep.c*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include "str.h"

#define FIND_STR        "-f"
#define REPLACE_STR     "-r"
#define DIFF_STR        "-d"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1

typedef enum {
  INVALID,
  FIND,
  REPLACE,
  DIFF
} CommandType;

/*
 * Fill out your functions here (If you need)
 */
/*define standard error messages (so I avoid typos)*/
/*not much commenting is done since these are self
explanatory (no global variables)*/
/*for when input line in stdin is too long*/
void inputLineTooLong(){
  fprintf(stderr, "Error: input line is too long\n");
  return;
}
/*for when input line of a file (name) within DoDiff is 
too long*/
void inputLineFileTooLong(const char* name){
  fprintf(stderr,"Error: input line %s is too long\n",name);
  return;
}
/*for when argument to long*/
void argTooLong(){
  fprintf(stderr, "Error: argument is too long\n");
  return;
}
/*for when empty string replacement was attempted for
DoReplace()*/
void emptyString(){
  fprintf(stderr, "Error: Can't replace an empty substring\n");
  return;
}
/*when a fileopen (name) has failed*/
void failOpen(const char* name){
  fprintf(stderr, "Error: failed to open file %s\n",name);
  return;
}
/*When file (name) ends early at line (line)*/
void fileEndsEarly(const char* name, unsigned int line){
  fprintf(stderr, "Error: %s ends early at line %u\n",name,line);
  return;
}
/* replaceLine replaces one line (str)'s occurence of
pcFrom to pcTo (used in DoReplace) (no global variable is used
or changed)*/
void replaceLine(char* str, const char* pcFrom , const char* pcTo){
  size_t psize = 0;
  size_t unit = sizeof('a');

  char* p = (char*) malloc(1*unit);
  *p = '\0';

  size_t toLen = StrGetLength(pcTo);
  size_t fromLen = StrGetLength(pcFrom);

  char* cur = str;
  /*the algorithm waits untill the first [toLen] substring is
  exactly pcTo, and changes.*/

  while(*cur){
    if(StrSearch(cur,pcFrom) == cur){
      psize += toLen;
      p = realloc(p,unit*psize+1);
      for(int i = 0;i<toLen;i++){
        *(p+psize-toLen+i) = *(pcTo + i);
      }
      cur += fromLen;
    }
    else{
      psize += 1;
      p = realloc(p,unit*psize+1);
      *(p+psize-1) = *cur;
      cur++;
    }
  }
  *(p+psize) = '\0';

  printf("%s",p);
  free(p);
  return;
}

/*--------------------------------------------------------------------*/
/* PrintUsage()
   print out the usage of the Simple Grep Program                     */
/*--------------------------------------------------------------------*/
void 
PrintUsage(const char* argv0) 
{
  const static char *fmt = 
    "Simple Grep (sgrep) Usage:\n"
    "%s [COMMAND] [OPTIONS]...\n"
    "\nCOMMNAD\n"
    "\tFind: -f [search-string]\n"
    "\tReplace: -r [string1] [string2]\n"
    "\tDiff: -d [file1] [file2]\n";

  printf(fmt, argv0);
}
/*-------------------------------------------------------------------*/
/* DoFind()
   Your task:
   1. Do argument validation 
   - String or file argument length is no more than 1023
   - If you encounter a command-line argument that's too long, 
   print out "Error: argument is too long"
   
   2. Read the each line from standard input (stdin)
   - If you encounter a line larger than 1023 bytes, 
   print out "Error: input line is too long" 
   - Error message should be printed out to standard error (stderr)
   
   3. Check & print out the line contains a given string (search-string)
      
   Tips:
   - fgets() is an useful function to read characters from file. Note 
   that the fget() reads until newline or the end-of-file is reached. 
   - fprintf(stderr, ...) should be useful for printing out error
   message to standard error

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
/*DoFind: Find and print (to stdout) all occurrences of pcSearch in
stdin (global variable MAX_STR_LEN is used, and no global variable is
changed.)*/
int
DoFind(const char *pcSearch)
{
  char buf[MAX_STR_LEN + 2]; 
  int len  = 0;
  /* 
   *  Fill out your variables here 
   */
   
  /* Read the line by line from stdin, Note that this is an example */

   /* Argument Validation*/
  if(StrGetLength(pcSearch) > MAX_STR_LEN){
    argTooLong();
    return FALSE;
  }

  /*traverse through stdin stream*/
  while (fgets(buf, sizeof(buf), stdin)) {
    len  = StrGetLength(buf);
    /* check input line length */
    if (StrGetLength(buf) > MAX_STR_LEN) {
      inputLineTooLong();
      return FALSE;
    }
    else if(StrSearch(buf,pcSearch)){
      if(*(buf+len-1) != '\n'){
      printf("%s\n",buf);
      }
      else{
        printf("%s",buf);
      }
    }
    /* TODO: fill out this function */
      
  }
  /*printf("\n");*/

   
  return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoReplace()
   Your task:
   1. Do argument validation 
      - String length is no more than 1023
      - If you encounter a command-line argument that's too long, 
        print out "Error: argument is too long"
      - If word1 is an empty string,
        print out "Error: Can't replace an empty substring"
      
   2. Read the each line from standard input (stdin)
      - If you encounter a line larger than 1023 bytes, 
        print out "Error: input line is too long" 
      - Error message should be printed out to standard error (stderr)

   3. Replace the string and print out the replaced string

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
/*DoReplace: Read stdin and print it will all occurences of pcString1
 changed to pcString2 (global variable MAX_STR_LEN is used, and no 
 global variable is changed.)*/
int
DoReplace(const char *pcString1, const char *pcString2)
{
  /* TODO: fill out this function */  
  char buf[MAX_STR_LEN + 2]; 

  if(StrGetLength(pcString1) > MAX_STR_LEN){
    argTooLong();
    return FALSE;
  }

  if(StrGetLength(pcString2) > MAX_STR_LEN){
    argTooLong();
    return FALSE;
  }

  if(*pcString1 == '\0'){
    emptyString();
    return FALSE;
  }

  while (fgets(buf, sizeof(buf), stdin)) {
    /* check input line length */
    if (StrGetLength(buf) > MAX_STR_LEN) {
      inputLineTooLong();
      return FALSE;
    }
    /*printf("%li",ftell(stdin));
    printf("\n<<%s\n>>",buf);*/
    replaceLine(buf,pcString1,pcString2);
    /* TODO: fill out this function */
      
  }
  return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoDiff()
   Your task:
   1. Do argument validation 
     - file name length is no more than 1023
     - If a command-line argument is too long, 
       print out "Error: argument is too long" to stderr

   2. Open the two files
      - The name of files are given by two parameters
      - If you fail to open either file, print out error messsage
      - Error message: "Error: failed to open file [filename]\n"
      - Error message should be printed out to stderr

   3. Read the each line from each file
      - If you encounter a line larger than 1023 bytes, 
        print out "Error: input line [filename] is too long" 
      - Error message should be printed out to stderr

   4. Compare the two files (file1, file2) line by line 

   5. Print out any different line with the following format
      file1@linenumber:file1's line
      file2@linenumber:file2's line

   6. If one of the files ends earlier than the other, print out an
      error message "Error: [filename] ends early at line XX", where
      XX is the final line number of [filename].

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
/*DoDiff: read two files file1 and file 2, and print line-by-line
where differences are found (global variable MAX_STR_LEN is used, and 
no global variable is changed.)*/
int
DoDiff(const char *file1, const char *file2)
{
  /* TODO: fill out this function */  
  /*1. argument validation*/
  if(StrGetLength(file1) > MAX_STR_LEN 
  || StrGetLength(file2) > MAX_STR_LEN){
  argTooLong();
  return FALSE;
  }



  /*2. Open Files*/
  FILE* f1 = fopen(file1,"r");
  FILE* f2 = fopen(file2,"r");
  int len1 = 0;
  int len2 = 0;

  if(f1 == f2 || file1 == file2){ /*same file: trivial result*/
    return TRUE;
  }

  if(f1 == NULL){
    failOpen(file1);
    return FALSE;
  }
  if(f2 == NULL){
    failOpen(file2);
    return FALSE;
  }

  /*3. Read each file line by line*/

  char buf1[MAX_STR_LEN + 2] = {0}; 
  char buf2[MAX_STR_LEN + 2] = {0};
  unsigned int line = 1;


  /* Read the line by line from stdin*/
  
  while (!feof(f1) && !feof(f2)) {
    /* since fgets does not cange buf1/2 when null is read,
    we need to initialize every time.*/
    for (int i = 0 ; i <MAX_STR_LEN+2;i++){
      *(buf1+i) = 0;
      *(buf2+i) = 0;
    }
    fgets(buf1, sizeof(buf1), f1) ;
    fgets(buf2, sizeof(buf2), f2) ;


    /*Checks for Empty Files. if file is empty, no further comparison
    is required*/
    /*if(feof(f1)||feof(f2)){
      line ++;
      break;
    }*/
    /*printf("[%d] %s||%d (%d)",line,buf1,*buf2,feof(f2));*/
    if(*buf1 == '\0' && feof(f1)){
      if (line == 1){return FALSE;}
      break;
    }
    if(*buf2 == '\0' && feof(f2)){
      if (line == 1){return FALSE;}
      break;
    }


    /*printf("%d%d,%d%d\n",*buf1,*(buf1+1),*buf2,*(buf2+1));*/

    /*if line too long, return error*/
    if (StrGetLength(buf1) > MAX_STR_LEN) {
      inputLineFileTooLong(file1);
      return FALSE;
    }
    if (StrGetLength(buf2) > MAX_STR_LEN) {
      inputLineFileTooLong(file2);
      return FALSE;
    }
    

    /*5. Print out any different line with the following format
      file1@linenumber:file1's line
      file2@linenumber:file2's line*/
    len1 = StrGetLength(buf1);
    len2 = StrGetLength(buf2);

    if(*(buf1+len1-1) != '\n'){
      /*printf("%s",buf1);*/
      *(buf1+len1) = '\n';
      *(buf1+len1+1) = '\0';
      len1++;
    }

    if(*(buf2+len2-1) != '\n'){
      *(buf2+len2) = '\n';
      *(buf2+len2+1) = '\0';
      len2++;
    }

    if (StrCompare(buf1,buf2)){
      printf("%s@%u:%s",file1,line,buf1);
      printf("%s@%u:%s",file2,line,buf2);
    }
    /*
    if (StrCompare(buf1,buf2)){
      if(*(buf1+StrGetLength(buf1)-1) != '\n'){
        printf("%s@%u:%s\n",file1,line,buf1);
      }
      else{printf("%s@%u:%s",file1,line,buf1);}
      if(*(buf2+StrGetLength(buf2)-1) != '\n'){
        printf("%s@%u:%s\n",file2,line,buf2);
      }
      else{printf("%s@%u:%s",file2,line,buf2);}
    }
    */

  line ++;
  }

  /*printf("\n\n>>%lu,%d<<\n\n",StrGetLength(buf2),
  *(buf2+StrGetLength(buf2)-1));*/

  /*if one ended early, return error*/
  /*printf("%d,%d",*buf1,*buf2);*/
  if (!feof(f1)){ /*file 2 ended early */
    if(StrGetLength(buf2) == 1 && *buf2 == '\n'){
      fileEndsEarly(file2,line-2);
      return FALSE;
    }
    fileEndsEarly(file2,line-1);
    return FALSE;
  }    

  if(!feof(f2)){ /*still data left in file2 -> file 1 ended early*/
    if(StrGetLength(buf1) == 1 && *buf1 == '\n'){
      fileEndsEarly(file1,line-2);
      return FALSE;
    }
    fileEndsEarly(file1,line-1);
    return FALSE;
  }
  fclose(f1);
  fclose(f2);
  return TRUE;
}




/*-------------------------------------------------------------------*/
/* CommandCheck() 
   - Parse the command and check number of argument. 
   - It returns the command type number
   - This function only checks number of argument. 
   - If the unknown function is given or the number of argument is 
   different from required number, this function returns FALSE.
   
   Note: You SHOULD check the argument rule later                    */
/*-------------------------------------------------------------------*/ 
int
CommandCheck(const int argc, const char *argv1)
{
  int cmdtype = INVALID;
   
  /* check minimum number of argument */
  if (argc < 3)
    return cmdtype;
   
  /* check command type */ 
  if (strcmp(argv1, FIND_STR) == 0) {
    if (argc != 3)
      return FALSE;    
    cmdtype = FIND;       
  }
  else if (strcmp(argv1, REPLACE_STR) == 0) {
    if (argc != 4)
      return FALSE;
    cmdtype = REPLACE;
  }
  else if (strcmp(argv1, DIFF_STR) == 0) {
    if (argc != 4)
      return FALSE;
    cmdtype = DIFF;
  }
   
  return cmdtype;
}
/*-------------------------------------------------------------------*/
int 
main(const int argc, const char *argv[]) 
{
  int type, ret;
   
  /* Do argument check and parsing */
  if (!(type = CommandCheck(argc, argv[1]))) {
    fprintf(stderr, "Error: argument parsing error\n");
    PrintUsage(argv[0]);
    return (EXIT_FAILURE);
  }
  
  /*comment for checking argv values. no code (that is not comment)
  in main was changed.*/
  /*
    int i;
    printf("%d\n",argc);
    for(i=0;i<argc;i++)
    {
        printf("arg %d: %s\n",i,argv[i]);
    }
  */

  /* Do appropriate job */
  switch (type) {
  case FIND:
    ret = DoFind(argv[2]);
    break;
  case REPLACE:
    ret = DoReplace(argv[2], argv[3]);
    break;
  case DIFF:
    ret = DoDiff(argv[2], argv[3]);
    break;
  } 

  return (ret)? EXIT_SUCCESS : EXIT_FAILURE;
}