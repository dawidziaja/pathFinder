#include "hashtable.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tableEntry {
    unsigned long key;
    char* item;
};
struct hashTable {
    struct tableEntry** entries;
    unsigned int size;
};

int addEntry(struct hashTable* table, char* item){
    unsigned int index;
    //generate a key for the item
    unsigned long key = genHash(item);
    //allocate some memory for our new entry
    struct tableEntry* newEntry = malloc(sizeof(struct tableEntry));

    if ( newEntry == NULL ){
        fprintf(stderr, "Malloc fail!\n");
        return -1;
    }
    //allocate the string length + 1 for null term byte 
    newEntry->item = malloc((strlen(item)+1)*sizeof(char));

    if(newEntry->item == NULL){
        fprintf(stderr, "Malloc fail!\n");
        return -1;
    }

    //copy over item name from input to our new table entry
    strcpy(newEntry->item, item);

    //get the starting index of the table
    index = key % (table->size);

    //increment the index until a null entry is found.
    while((table->entries[index] != NULL) && (table->entries[index]->key != key)){
        index = ( index + 1 ) % (table->size);
    }
    newEntry->key=key;
    table->entries[index]=newEntry;

    return (int)index;
}

int findEntry(struct hashTable* table, unsigned long key)
{
    unsigned int index = (key % (table->size));
    while( table->entries[index] != NULL ){
        if( table->entries[index]->key == key ){
            return (int)index;
        }
        //wraps around search if i exceeds the table size.
        index = ( index + 1 ) % (table->size);

    }
    return -1;
}

char* getText(struct hashTable* table, unsigned int id)
{
    return table->entries[id]->item;
}

struct hashTable* createTable(unsigned int size){
    struct hashTable* table = malloc(sizeof(struct hashTable));

    if(table == NULL){
        fprintf(stderr, "Malloc fail!\n");
        return NULL;
    }

    table->size = size;
    table->entries = calloc(size, sizeof(struct tableEntry*));

    if(table->entries == NULL){
        fprintf(stderr, "Malloc fail!\n");
        return NULL;
    }

    return table;

}

void printTable ( struct hashTable* table )
{

    for (unsigned int i = 0; i < table->size; i++ )
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
        hash = ((hash << 5) + hash) + (unsigned long)c; /* hash * 33 + c */

    return hash;
}

void destroyTable(struct hashTable* table)
{
    for(unsigned int i=0; i<table->size; i++ ){
        if(table->entries[i] != NULL){
            free( table->entries[i]->item );
            free( table->entries[i] );
        }
    }
    free(table->entries);
    free(table);
}
