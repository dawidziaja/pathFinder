#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE ( 24 )


struct tableEntry {
    unsigned long key;
    char* item;
};


/*

if table[index].key is  

*/
int addEntry(struct tableEntry** table, char* item){
    int index;
    unsigned long key = genHash(item);
    struct tableEntry* newEntry = malloc(sizeof(struct tableEntry));

    if ( newEntry == NULL )
        return -1;

    newEntry->item = malloc((strlen(item)+1)*sizeof(char));
    strcpy(newEntry->item, item);
    index = key%TABLE_SIZE;
    /*Increment table index until an empty entry is found*/
    while(table[index] != NULL && table[index]->key != key){
        index++;
        if(index==TABLE_SIZE){
            index=0;
        }
        /*FIX LATER, POSSIBLE INFINITE LOOP IF TABLE TOO FULL INNIT*/
    }
    newEntry->key=key;
    table[index]=newEntry;
    return index;
}

int findEntry(struct tableEntry** table, unsigned long key)
{
    unsigned long index = key % TABLE_SIZE;
    while( table[index] != NULL ){
        if( table[ index ]->key == key )
            return index;

        index = ( index + 1 ) % TABLE_SIZE;
        /*IF ENTRY DOES NOT EXIST AND TABLE IS FULL THIS WILL LOOP FOREVER. (although final element should always be null so its chillin)*/
    }
    return -1;
}

char* getText(struct tableEntry** table, int id)
{
    return table[id]->item;
}

struct tableEntry** createTable(){
    return calloc(TABLE_SIZE, sizeof(struct tableEntry*));
}

void printTable ( struct tableEntry ** self )
{
    int i;

    for ( i = 0; i < TABLE_SIZE; i++ )
        if ( self [ i ] != NULL )
            printf ( "Table index %d holds item %s with hash %lu\n",
                i, self [ i ]->item, self [ i ]->key );
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

void destroyTable(struct tableEntry** self)
{
    int i;
    for( i=0; i<TABLE_SIZE; i++ ){
        free( self[i]->item );
        free( self[i] );
    }
    free( self );
}