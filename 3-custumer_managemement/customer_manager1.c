#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*Uncomment and use the following code if you want*/

#define UNIT_ARRAY_SIZE 64

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              
          // # of stored items, needed to determine
          // # whether the array should be expanded
          // # or not
  int cursor; // cursor for where to add a custumer next.
};

void printUserInfo(struct UserInfo *u,int num){
  if (u->name == NULL){printf("\t[%d] [UserInfo] <empty>\n",num); return;}
  printf("\t[%d] [UserInfo] name: %s, id: %s, purchase: %d\n",num,u->name,u->id,u->purchase);
  return;
}

void printDB(struct DB *d){
  printf("[DB] array size: %d, numItems: %d\nEntries:\n",d->curArrSize,d->numItems);
  for(int i = 0; i<d->curArrSize; i++){
    printUserInfo(&(d->pArray)[i],i);
  }
  printf("<end>\n");
  return;
}

/*--------------------------------------------------------------------*/
DB_T
CreateCustomerDB(void)
{
  /*Uncomment and use the following implementation if you want*/
  DB_T d;

  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 64 elements

  d->pArray = (struct UserInfo *)calloc(d->curArrSize,
               sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);
    free(d);
    return NULL;
  }

  d->numItems = 0;
  d->cursor = 0;
  return d;

  /*return NULL;*/
}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  /* fill out this function */
  if(d == NULL){return;}

  for(int i = 0; i<d->curArrSize;i++){
    free((d->pArray)[i].id);
    free((d->pArray)[i].name);
  }

  free(d->pArray);
  free(d);  
  /*assert(0);
  return NULL;*/
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  // int addHere = 0;
  /*argument validation*/
  if(!(d&&id&&name)){return -1;}
  if(purchase <= 0){return -1;}
  /*check through pArray for overlapping id or name*/
  for(int i = 0; i<d->curArrSize;i++){
    if(d->pArray[i].name == NULL){continue;}
    
    // printf("comparing [%s,%s]",(d->pArray)[i].id,id);
    if( !strcmp((d->pArray)[i].id,id) || 
    !strcmp((d->pArray)[i].name,name) ) {
      /*printf("HIT\n");*/
      return -1;
    }
  }

  if(d->numItems >= d->curArrSize){
    d->curArrSize += UNIT_ARRAY_SIZE;
    d->pArray = realloc(d->pArray,d->curArrSize*sizeof(struct UserInfo));
    printf("Array Expanded : from %d to %d\n",d->curArrSize-UNIT_ARRAY_SIZE,d->curArrSize);
    // printDB(d);
  }

  while((d->pArray)[d->cursor].name){
    if(++d->cursor >= d->curArrSize){d->cursor = 0;}
  }
  
  (d->pArray)[d->cursor].purchase = purchase;

  (d->pArray)[d->cursor].name = (char*)calloc(strlen(name)+1,sizeof(char));
  (d->pArray)[d->cursor].id = (char*)calloc(strlen(id)+1,sizeof(char));

  strcpy((d->pArray)[d->cursor].name,name);
  strcpy((d->pArray)[d->cursor].id,id);
  ++(d->numItems);
  // printf("\tadded id: %s, name: %s at %d\n",id,name,d->cursor);
  // d->cursor += 3; //uncomment this line for cursor shuffling test
  if(++d->cursor >= d->curArrSize){d->cursor = 0;}

  // printDB(d);
  //assert(0);
  return (0);
}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  if(d == NULL){return -1;}
  if(id == NULL){return -1;}
  // printDB(d);
  for(int i = 0; i<d->curArrSize;i++){
    // printf("%d\n",i);
    if(d->pArray[i].name == NULL){continue;}
    // printf("%c\n",*(((d->pArray)[i]).id));
    if( !strcmp((d->pArray)[i].id,id) ) {
      free(d->pArray[i].id);
      free(d->pArray[i].name);
      d->pArray[i].id = NULL;
      d->pArray[i].name = NULL;
      --(d->numItems);
      return 0;
      }
    // printf("%d\n",i);
  }  
  // printf("loop exited");

  /* fill out this function */
  // assert(0);
  return (-1); //no such id was found
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  if(d == NULL){return -1;}
  if(name == NULL){return -1;}
  for(int i = 0; i<d->curArrSize;i++){
    if(d->pArray[i].name == NULL){continue;}
    if( !strcmp((d->pArray)[i].name,name) ) {
      free(d->pArray[i].id);
      free(d->pArray[i].name);
      d->pArray[i].id = NULL;
      d->pArray[i].name = NULL;
      --(d->numItems);
      return 0;
      }
  }  

  // assert(0);
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  if(d == NULL){return -1;}
  if(id == NULL){return -1;}

  for(int i = 0; i<d->curArrSize;i++){
    if(d->pArray[i].name == NULL){continue;}
    if( !strcmp((d->pArray)[i].id,id) ) {
      return(d->pArray[i].purchase);
      }
  }  

  /* fill out this function */
  return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  if(d == NULL){return -1;}
  if(name == NULL){return -1;}

  for(int i = 0; i<d->curArrSize;i++){
    if(d->pArray[i].name == NULL){continue;}
    if( !strcmp((d->pArray)[i].name,name) ) {
      return(d->pArray[i].purchase);
      }
  }  

  /* fill out this function */
  return (-1);
}
/*--------------------------------------------------------------------*/

int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  if(d == NULL){return -1;}
  if(fp == NULL){return -1;}

  int total = 0;

  for(int i = 0; i<d->curArrSize;i++){
    if(d->pArray[i].name == NULL){continue;}
    total += fp(d->pArray[i].id,d->pArray[i].name,d->pArray[i].purchase);
  }
  return total;
}
