#ifndef IO_H /*Include guard*/
#define IO_H

struct mapStruct{
	struct hashTable* table;
	struct Graph* graph;
};

struct mapStruct* loadEnergy(const char*);
unsigned int loadPairs(struct Graph* g, struct hashTable* ht, const char* fn);

#endif /*_IO_H*/
