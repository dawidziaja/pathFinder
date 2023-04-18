#ifndef IO_H /*Include guard*/
#define IO_H

#include "graph.h"
#include "hashtable.h"

struct mapStruct{
	struct hashTable* table;
	struct Graph* graph;
};

struct mapStruct* loadEnergy(const char*);
unsigned int loadPairs(struct Graph* g, struct hashTable* ht, const char* fn);


//This is the most straight forward way of returning the hashtable and the graph
//from loadEnergy that I could think of. probably not the best.

#endif /*_IO_H*/
