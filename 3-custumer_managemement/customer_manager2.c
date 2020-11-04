#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*Uncomment and use the following code if you want*/

#define UNIT_ARRAY_SIZE 64
const unsigned int INITIAL_HASH_SIZE = 64;
#define HASH_NUMBER 6151 /*hash prime number provided by https://planetmath.org/goodhashtableprimes*/

struct UserInfo {
  char *name;                 // customer name
  char *id;                   // customer id
  int purchase;               // purchase amount (> 0)
  unsigned int id_hash;                   // hash value
  unsigned int name_hash;
  struct UserInfo* id_next;   // next pointer when referenced with id 
  struct UserInfo* name_next; // next pointer when referenced with name
};

struct DB {
  struct UserInfo* (*id_ht);   // hashtable by id
  struct UserInfo* (*name_ht); // hashtable by name
  unsigned int hash_size;
  unsigned int numItems;             // number of items.
};

void printUserInfo(struct UserInfo *u,int table,int index){
  if (u == NULL){/*printf("\t[%d] [UserInfo] <empty>\n",num);*/ printf("\t[%d]<#%d> [UserInfo] <empty/NULL>\n",table,index);return;}
  printf("\t[%d]<#%d> [UserInfo] id: %s (id hash: %u), name: %s (name hash: %u), purchase: %d\n",table,index,u->id,u->id_hash,u->name,u->name_hash,u->purchase);
  return;
}

void printDB(struct DB *d){
  struct UserInfo *cur;
  printf("[DB] hash size: %d, numItems: %d. Entries (by id hash table):\n",d->hash_size,d->numItems);
  for(int i = 0; i<d->hash_size;i++){
    cur = (d->id_ht)[i];
    fflush(stdout);
    // if(cur==NULL){continue;}
    int j = 0;
    while(cur){
      // printf("line 46\n");
      printUserInfo(cur,i,j);
      cur = cur->id_next;
      ++j;
    }
    if(j != 0){printf("\n");}
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
  d->hash_size = INITIAL_HASH_SIZE; // start with 64 elements
  d->numItems = 0;

  d->id_ht   = (struct UserInfo **)calloc(d->hash_size,sizeof(struct UserInfo *));
  
  d->name_ht = (struct UserInfo **)calloc(d->hash_size,sizeof(struct UserInfo *));

  if (!(d->id_ht) || !(d->hash_size) ) {
    fprintf(stderr, "Can't allocate a memory for hash tables");
    free(d);
    return NULL;
  }
  // printf("hello")
  // printf("Initialized DB at: %p, with id_ht: %p, name_ht: %p\n",(void *)d,(void *)d->id_ht,(void *)d->name_ht);
  return d;
}
/*--------------------------------------------------------------------*/
void DestroyUserInfo(struct UserInfo *u){
  if(!u){return;}
  free(u->name);
  free(u->id);
  free(u);
  return;
}

void
DestroyCustomerDB(DB_T d) //should iterate through & free from one of the tables.
{
  /* fill out this function */
  struct UserInfo *cur;
  struct UserInfo *nextcur;

  if(!d){return;}
  
  for(int i = 0; i<d->hash_size;i++){
    cur = (d->name_ht)[i];
    // if(cur==NULL){continue;}
    while(cur){
      nextcur = cur->name_next;
      DestroyUserInfo(cur);
      cur = nextcur;
    }
  }

  free(d->id_ht);
  free(d->name_ht);
  

  free(d);  
  return ;
}
/*--------------------------------------------------------------------*/
unsigned int hash(const char *c){
  unsigned int h = 0;
  for(int i = 0; i<strlen(c) ; i++){
    h = h*HASH_NUMBER + c[i];
  }
  return h;
}
/*--------------------------------------------------------------------*/
struct UserInfo** search_id(DB_T d, const char *id){//custom searches return references, so they can be modified.
  unsigned int target_hash = hash(id);
  // printf("\n\ntarget hash: %u\n",target_hash);
  struct UserInfo** cur = &(d->id_ht[target_hash & (d->hash_size-1)]);
  // if(!cur){printf("cur is NULL at %u, id_ht is at %p\n",target_hash & (d->hash_size-1),(void*)d->id_ht);fflush(stdout);}
  while (*cur != NULL){
    // printf("comparing: %u and %u between id: %s and %s\n",cur->id_hash,target_hash,cur->id,id);
    if(((*cur)->id_hash == target_hash) && (!strcmp((*cur)->id,id))){return cur;}
    cur = &((*cur) -> id_next);
  }
  return cur;
}

struct UserInfo** search_name(DB_T d, const char *name){ 
  unsigned int target_hash = hash(name);
  // printf("\n\ntarget hash: %u\n",target_hash);
  struct UserInfo** cur = &(d->name_ht[target_hash & (d->hash_size-1)]);
  // if(!cur){printf("cur is NULL at %u, name_ht is at %p\n",target_hash & (d->hash_size-1),(void*)d->name_ht);fflush(stdout);}
  while (*cur != NULL){
    // printf("comparing: %u and %u between name: %s and %s\n",cur->name_hash,target_hash,cur->name,name);
    if(((*cur)->name_hash == target_hash) && (!strcmp((*cur)->name,name))){return cur;}
    cur = &((*cur) -> name_next);
  }
  return cur;
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase){
  if(!(d&&id&&name)){return -1;}
  if(purchase <= 0){return -1;}

  /*TODO: search for ovelapping id or name*/
  struct UserInfo** id_dest = search_id(d,id);
  struct UserInfo** name_dest = search_name(d,name);

  if(*(id_dest)){/*printf("duplicate id.\n");*/return -1;}
  if(*(name_dest)){/*printf("duplicate name.\n");*/return -1;}

  struct UserInfo* u = (struct UserInfo*)malloc(sizeof(struct UserInfo));
  // struct UserInfo* cur;
  
  u->purchase = purchase;

  u->name = (char*)calloc(strlen(name)+1,sizeof(char));
  u->id = (char*)calloc(strlen(id)+1,sizeof(char));
  
  strcpy(u->name,name);
  strcpy(u->id,id);
  
  u->id_hash = hash(u->id);
  u->name_hash = hash(u->name);

  u->id_next = NULL;
  u->name_next = NULL;

  *(id_dest) = u;
  *(name_dest) = u;
  
  // printf("REGISTER: name hash : %u, index: %u\n",u->name_hash, (u->name_hash & (d->hash_size-1)));
  // printf("REGISTER:  id  hash : %u, index: %u\n",u->id_hash, (u->id_hash & (d->hash_size-1)));
  // fflush(stdout);


  // u->id_next = (d->id_ht)[u->id_hash & (d->hash_size-1)];
  // (d->id_ht)[u->id_hash & (d->hash_size-1)] = u;
  
  // u->name_next = (d->name_ht)[u->name_hash & (d->hash_size-1)];
  // (d->name_ht)[u->name_hash & (d->hash_size-1)] = u;

  ++(d->numItems);

  // printDB(d);

  return 0;

}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  // printf("Before Delete ID:\n");
  // printDB(d);
  if(!(d&&id)){return -1;}
  struct UserInfo ** res_reference_id = search_id(d,id);
  if(! *(res_reference_id)){return -1;}

  struct UserInfo ** res_reference_name = search_name(d,(*res_reference_id)->name);

  struct UserInfo * idnext = (*res_reference_id)->id_next;
  struct UserInfo * namenext = (*res_reference_name)->name_next;
  DestroyUserInfo(*res_reference_id);
  *res_reference_id = idnext;
  *res_reference_name = namenext;
  --(d->numItems);
  // printf("After Delete ID:\n");
  // printDB(d);
  return (0);
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  if(!(d&&name)){return -1;}
  // printDB(d);
  struct UserInfo ** res_reference_name = search_name(d,name);
  if(! *(res_reference_name)){return -1;}

  struct UserInfo ** res_reference_id = search_id(d,(*res_reference_name)->id);

  struct UserInfo * idnext = (*res_reference_id)->id_next;
  struct UserInfo * namenext = (*res_reference_name)->name_next;
  DestroyUserInfo(*res_reference_name);
  *res_reference_id = idnext;
  *res_reference_name = namenext;
  --(d->numItems);
  return (0);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
  if(!(d&&id)){return -1;}
  struct UserInfo * result = (*search_id(d,id));
  if(!result){return -1;}

  return result->purchase;
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
  if(!(d&&name)){return -1;}
  struct UserInfo * result = (*search_name(d,name));
  if(!result){return -1;}

  return result->purchase;
}
/*--------------------------------------------------------------------*/
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  if(!(d&&fp)){return -1;}
  struct UserInfo *cur;
  int total = 0;
  
  for(int i = 0; i<d->hash_size;i++){
    cur = (d->name_ht)[i];
    while(cur){
      total += fp(cur->id,cur->name,cur->purchase);
      cur = cur->name_next;
    }
  }
  return total;
}
