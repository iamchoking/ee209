/*20190673 Hyungho Choi custumer_manager1.c*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define UNIT_ARRAY_SIZE 64

/*UserInfo(array version) Holds User Info: name (string), id (string),
purchase (int)*/
struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

/*DB (array version) Forms the database of UserInfo's. pArray: pointer 
to an array of UserInfo Datas
curArrSize: current array size, int. numItems: number of stored Items 
(int), cursor (int): next recommended index to add (for time efficiency)
*/
struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              
          // # of stored items, needed to determine
          // # whether the array should be expanded
          // # or not
  int cursor; // cursor for where to add a custumer next.
};

/*DESCRIPTION
printUserInfo prints a UserInfo Node
PARAMETERS
u: UserInfo pointer to print
num: (int) a prefix number
Reads from: arguments
Writes to: stdout
Affects no global varaibles*/
void printUserInfo(struct UserInfo *u,int num){
  if (u->name == NULL){printf("\t[%d] [UserInfo] <empty>\n",num); 
  return;}
  printf("\t[%d] [UserInfo] name: %s, id: %s, purchase: %d\n",num,
  u->name,u->id,u->purchase);
  return;
}

/*
DESCRIPTION
prints entire DB by iterating thgough the array.
PARAMETERS
d: pointer to DB struct to print
Returns: void 
Reads from: arguements
Writes To: stdout
Affected Global Variables: None
*/
void printDB(struct DB *d){
  printf("[DB] array size: %d, numItems: %d\nEntries:\n",d->curArrSize,
  d->numItems);
  for(int i = 0; i<d->curArrSize; i++){
    printUserInfo(&(d->pArray)[i],i);
  }
  printf("<end>\n");
  return;
}

/*--------------------------------------------------------------------*/
/*
DESCRIPTION
Creates and initializes a customer DB
PARAMETERS
void
Returns: pointer to the created DB (NULL if failed)
Reads from: nothing
Writes To: nothing
Affected Global Variables: references UNIT_ARRAY_SIZE for array creation.
*/
DB_T CreateCustomerDB(void){
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

}
/*--------------------------------------------------------------------*/
/*
DESCRIPTION
frees a DB data without leaks.
PARAMETERS
d: DB pointer to delete
Returns:void
Reads from: arguments
Writes To: nothing
Affected Global Variables: none
*/
void DestroyCustomerDB(DB_T d){
  /* fill out this function */
  if(d == NULL){return;}

  for(int i = 0; i<d->curArrSize;i++){
    if(!d->pArray){break;}
    free((d->pArray)[i].id);
    free((d->pArray)[i].name);
  }

  free(d->pArray);
  free(d);  
}
/*--------------------------------------------------------------------*/
/*
DESCRIPTION
Creates and registers a UserInfo Node to the given DB expands when out
of space.
PARAMETERS
d: DB pointer to add the node to
id: (string) id of new node added
name: (string) id of new node added
purchase: (int) purchace value of new noded added
Returns: 0 if sucess, -1 if fail
Reads from: arguments
Writes To: stderr when memory allocation fail
Affected Global Variables: references UNIT_ARRAY_SIZE to increase array
size.
*/
int RegisterCustomer(DB_T d, const char *id, const char *name, const 
int purchase){
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

  if(d->numItems >= d->curArrSize){ //array expansion
    d->curArrSize += UNIT_ARRAY_SIZE;
    d->pArray = realloc(d->pArray,d->curArrSize*
    sizeof(struct UserInfo));
    if (d->pArray == NULL) {
      fprintf(stderr, "Can't allocate expanded array for DB_T\n");
      DestroyCustomerDB(d);
      return -1;
    }
    // printf("Array Expanded : from %d to %d\n",
    // d->curArrSize-UNIT_ARRAY_SIZE,d->curArrSize);
    // printDB(d);
  }

  while((d->pArray)[d->cursor].name){
    if(++d->cursor >= d->curArrSize){d->cursor = 0;}
  }
  
  (d->pArray)[d->cursor].purchase = purchase;

  (d->pArray)[d->cursor].name = (char*)calloc(strlen(name)+1,
  sizeof(char));
  (d->pArray)[d->cursor].id = (char*)calloc(strlen(id)+1,sizeof(char));
  if (!((d->pArray)[d->cursor].name&&(d->pArray)[d->cursor].id)) {
    fprintf(stderr,
    "Can't allocate new id / cursor value for new node\n");
    DestroyCustomerDB(d);
    return -1;
  }

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
/*
DESCRIPTION
Searches and Unregisters (deletes) UserInfo Data with given id
PARAMETERS
d: DB pointer process deletion
id: (string) id of node to delete
Returns: 0 if sucess, -1 if fail
Reads from: arguments
Affected Global Variables: none
*/
int UnregisterCustomerByID(DB_T d, const char *id) {
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
/*
DESCRIPTION
Searches and Unregisters (deletes) UserInfo Data with given name
PARAMETERS
d: DB pointer process deletion
name: (string) name of node to delete
Returns: 0 if sucess, -1 if fail
Reads from: arguments
Affected Global Variables: none
*/
int UnregisterCustomerByName(DB_T d, const char *name) {
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
/*
DESCRIPTION
Searches and retrieves the purchase velue of a UserInfo data with 
given id
PARAMETERS
d: DB pointer to search
id: (string) id of node to search
Returns: purchase value if sucess, -1 if fail
Reads from: arguments
Affected Global Variables: none
*/
int GetPurchaseByID(DB_T d, const char* id) {
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
/*
DESCRIPTION
Searches and retrieves the purchase velue of a UserInfo data with 
given name
PARAMETERS
d: DB pointer to search
name: (string) name of node to search
Returns: purchase value if sucess, -1 if fail
Reads from: arguments
Affected Global Variables: none
*/
int GetPurchaseByName(DB_T d, const char* name) {
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

/*
DESCRIPTION
iterates through all of the database, and calculates the sum of each
output from a given function.
PARAMETERS
d: DB pointer to iterate
fp: (function pointer) function to calculate
Returns: calculated value if sucess, -1 if fail
Reads from: arguments
Affected Global Variables: none
*/
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp) {
  if(!(d&&fp)){return -1;}

  int total = 0;

  for(int i = 0; i<d->curArrSize;i++){
    if(d->pArray[i].name == NULL){continue;}
    total += fp(d->pArray[i].id,d->pArray[i].name,
    d->pArray[i].purchase);
  }
  return total;
}
