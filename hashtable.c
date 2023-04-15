#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tableEntry {
    unsigned long key;
    char* item;
};


struct hashTable {
    struct tableEntry** entries;
    int size;
};


/*

if table[index].key is  

*/
int addEntry(struct hashTable* table, char* item){
    int index;
    unsigned long key = genHash(item);
    struct tableEntry* newEntry = malloc(sizeof(struct tableEntry));

    if ( newEntry == NULL )
        return -1;

    newEntry->item = malloc((strlen(item)+1)*sizeof(char));
    strcpy(newEntry->item, item);
    index = key%(table->size);
    while(table->entries[index] != NULL && table->entries[index]->key != key){
        index++;
        if(index==table->size){
            index=0;
        }
    }
    newEntry->key=key;
    table->entries[index]=newEntry;
    return index;
}

int findEntry(struct hashTable* table, unsigned long key)
{
    unsigned long index = key % (table->size);
    while( table->entries[index] != NULL ){
        if( table->entries[ index ]->key == key )
            return index;

        index = ( index + 1 ) % (table->size);
        /*IF ENTRY DOES NOT EXIST AND TABLE IS FULL THIS WILL LOOP FOREVER. (although final element should always be null so its chillin)*/
    }
    return -1;
}

char* getText(struct hashTable* table, int id)
{
    return table->entries[id]->item;
}

struct hashTable* createTable(int size){
    struct hashTable* table = malloc(sizeof(struct hashTable));
    table->size = size;
    table->entries = calloc(size, sizeof(struct tableEntry*));
}

void printTable ( struct hashTable* table )
{
    int i;

    for ( i = 0; i < table->size; i++ )
        if ( table->entries [ i ] != NULL )
            printf ( "Table index %d holds item %s with hash %lu\n",
                i, table->entries [ i ]->item, table->entries [ i ]->key );
}

/*
djb2 by Dan Bernstein http://www.cse.yorku.ca/~oz/hash.html
A simple hashing algorithm with low collisions
*/
unsigned long genHash(char *str)
{
    unsigned long hash = 5381;
    char c;

    while ( ( c = *str++ ) != '\0' )
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void destroyTable(struct hashTable* table)
{
    int i;
    for( i=0; i<table->size; i++ ){
        free( table->entries[i]->item );
        free( table->entries[i] );
    }
    free(table->entries);
    free(table);
}