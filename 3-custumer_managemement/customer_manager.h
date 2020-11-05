/*20190673 Hyungho Choi custumer_manager.h*/

#ifndef CUSTOMER_MANAGER_H
#define CUSTOMER_MANAGER_H

/**********************
 * EE209 Assignment 3 *
 **********************/
/* customer_manager.h */

/* forward type definition for DB_T */
/* "struct DB" should be defined in customer_manager1.c,
    customer_manager2.c, and customer_manager3.c */
typedef struct DB* DB_T;

/* function pointer type definition */
typedef int (*FUNCPTR_T)(const char* id, const char* name,
						 const int purchase);

/* create and return a db structure */
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
DB_T CreateCustomerDB(void);

/* destory db and its associated memory */
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
void DestroyCustomerDB(DB_T d);

/* register a customer with (name, id, purchase) */
/*
DESCRIPTION
Creates and registers a UserInfo Node to the given DB expands array / hash when out
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
int  RegisterCustomer(DB_T d, const char *id,
		      const char *name, const int purchase);

/* unregister a customer with 'id' */
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
int UnregisterCustomerByID(DB_T d, const char *id);

/* unregister a customer with 'name' */
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
int UnregisterCustomerByName(DB_T d, const char *name);

/* get the purchase amount of a user whose ID is 'id' */
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
int GetPurchaseByID(DB_T d, const char* id);

/* get the purchase amount of a user whose name is 'name' */
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
int GetPurchaseByName(DB_T d, const char* name);

/* iterate all valid user items once, evaluate fp for each valid user
   and return the sum of all fp function calls */
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
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp);

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
void printDB(DB_T d); //added here for debugginf purposes.

#endif /* end of CUSTOMER_MANAGER_H */
