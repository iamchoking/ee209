#include <assert.h> /* to use assert() */
#include <stdio.h>
#include "str.h"
#include <string.h> /*only added here as placeholder.*/

/* Your task is: 
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/

/* Part 1 */
/*------------------------------------------------------------------------*/
size_t StrGetLength(const char* pcSrc)
{
  const char *pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc;
	
  while (*pcEnd) /* null character and FALSE are identical. */ /*same with while (*pcEnd != NULL)*/
    pcEnd++;

  return (size_t)(pcEnd - pcSrc);
}

/*------------------------------------------------------------------------*/
char* StrCopy(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  assert(pcSrc); /*make sure source pointer is valid*/

  char* dest = pcDest; /*make two "cursor" pointers to help along memory change*/
  char* cur = (char*) pcSrc;

  if(! *pcSrc){
    *(pcDest) = '\0';
    return pcDest;
  }

  while(*cur){
    *dest = *cur;
    /*printf("%c",*cur);*/
    dest++;
    cur++;
  }

  *(++dest) = '\0';
  

  return pcDest;
}

/*------------------------------------------------------------------------*/
int StrCompare(const char* pcS1, const char* pcS2)
{
  /* TODO: fill this function */
  assert (pcS1 && pcS2);
  /*used recursion for this function.*/

  /*first, handle trivial cases*/
  if(*pcS1 == '\0'){
    return -*pcS2;
  }

  if(*pcS2 == '\0'){
    return *pcS1;
  }

  /*now, handle non-trivial cases*/
  if(*pcS1 == *pcS2){ /*compare next if same (and neiter are null)*/
    return StrCompare(pcS1+1,pcS2+1);
  }
  else{ /*return difference if not same (and neither are null)*/
    return (*pcS1 - *pcS2);
  }
 
  /*return strcmp(pcS1, pcS2);*/
}
/*------------------------------------------------------------------------*/
char* StrSearch(const char* pcHaystack, const char *pcNeedle)
{
  /* TODO: fill this function */
  /*printf("%c",*(pcHaystack));*/
  if (*pcNeedle == '\0'){return (char*)pcHaystack;}

  int lenHaystack = (int) StrGetLength(pcHaystack);
  int lenNeedle = (int) StrGetLength(pcNeedle);

  for(int i = 0;i < lenHaystack-lenNeedle;i++){
    for(int j = 0 ; j<=lenNeedle ; j++){
      if(*(pcHaystack+i+j) != *(pcNeedle + j)){
        break;
      }
      /*printf("potential! %s,%c,%d    ",(pcHaystack+i),*(pcNeedle+j),j);*/
      if(j == lenNeedle-1){
        return (char*)(pcHaystack + i);
      }
    }
  }
 
  return 0;
}
/*------------------------------------------------------------------------*/
char *StrConcat(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  /*int srcLen = (int)StrGetLength(pcSrc);*/
  char* srcCur = (char*) pcSrc;
  char* destCur = (char*)(pcDest+StrGetLength(pcDest));

  while(*srcCur){
    *destCur = *srcCur;
    srcCur++;
    destCur++;
  }
  *(++destCur) = '\0';
  
  return pcDest;
}