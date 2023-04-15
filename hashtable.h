#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

/* Define hashtable struct */

struct hashTable;

struct hashTable* createTable(int size);

/*Adds an entry to the Table, returns index of item*/
int addEntry(struct hashTable*, char*);

/*Finds an entry with the passed hash in the table*/
int findEntry(struct hashTable*, unsigned long);

char* getText(struct hashTable*, int);

/* generates a hash for given variable */
unsigned long genHash(char*);

void printTable(struct hashTable*);

void destroyTable(struct hashTable*);

#endif /* _HASH_TABLE_H */
