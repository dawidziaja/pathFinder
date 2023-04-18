#include "io.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CITIES 50

int countCities(const char* fn);

FILE* loadFile(const char* fn);

FILE* loadFile(const char* fn){
	FILE* fp = NULL;
	if ( ( fp = fopen(fn, "r") ) == NULL ) {
		errorHandler("Failed to open file.");
    }
	return fp;
}
struct mapStruct* loadEnergy(const char* fn)
{
	char line[70];
    char city1[15];
    char city2[15];
    int distance;

	int cityCount = countCities(fn);

	FILE* fp = loadFile(fn);

	struct Graph* g = createGraph(cityCount);

	struct hashTable* table = createTable(cityCount);


	while(fgets(line, sizeof(line), fp) != NULL){
        sscanf(line, "%s\t%s\t%d[^\n]", city1, city2, &distance);

        if(findEntry(table, genHash(city1)) == -1){
            addNode(g, addEntry(table, city1));
        }
        if(findEntry(table, genHash(city2)) == -1){
            addNode(g, addEntry(table, city2));
        }
        addEdge(g, findEntry(table, genHash(city1)), 
        findEntry(table, genHash(city2)), distance);
    }
	fclose(fp);

	struct mapStruct* ms = malloc(sizeof(struct mapStruct));

	if(ms==NULL){
		errorHandler("Malloc fail!");
	}

	ms->graph = g;
	ms->table = table;
	return ms;
}


//I hate how i do this but it works so...
int countCities(const char* fn)
{
	FILE* fp = loadFile(fn);
	char line[70];
	char city1[15];
	char city2[15];
	int c1a, c2a;
	unsigned long c1h, c2h;
	unsigned long* cities = malloc(sizeof(unsigned long)*MAX_CITIES);
	int cityCount = 0;

	if(cities==NULL){
		errorHandler("Malloc fail!");
	}

    while(fgets(line, sizeof(line), fp) != NULL){
		c1a = 1;
		c2a = 1;
        sscanf(line, "%s\t%s[^\n]", city1, city2);

		c1h = genHash(city1); //use the hash function from the hashtable
		c2h = genHash(city2); //so we dont need to compare strings

		for(int i=0;i<cityCount;i++)
		{
			if(cities[i] == c1h){//compare hashes
				c1a = 0;
			}
			if(cities[i] == c2h){
				c2a = 0;
			}
		}
		if(c1a){
			cities[cityCount] = c1h;
			cityCount++;
		}
		if(c2a){
			cities[cityCount] = c2h;
			cityCount++;
		}
    }
	fclose(fp);
	free(cities);
	return cityCount;

}

void loadPairs(struct Graph* g, struct hashTable* ht, const char* fn){
	FILE* fp = loadFile(fn);

	//temporary buffers
	char line[70];
	char city1[15];
	char city2[15];

	while(fgets(line, sizeof(line), fp) != NULL){
		sscanf(line, "%s\t%s[^\n]", city1, city2);
		findShortestPath(g, ht, city1, city2);
	}

}