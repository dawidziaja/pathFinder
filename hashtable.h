#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

/* Define hashtable struct */
struct tableEntry;

struct tableEntry** createTable(void);

/*Adds an entry to the Table, returns index of item*/
int addEntry(struct tableEntry**, char*);

/* returns a pointer to the value in the hash table associated with the given key */
struct tableEntry* getItem(unsigned long);

/*Finds an entry with the passed hash in the table*/
int findEntry(struct tableEntry**, unsigned long);

char* getText(struct tableEntry**, int);

/* generates a hash for given variable */
unsigned long genHash(char*);

void printTable(struct tableEntry**);

void destroyTable(struct tableEntry**);

#endif /* _HASH_TABLE_H */
