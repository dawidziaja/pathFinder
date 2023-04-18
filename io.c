#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CITIES 50

int countCities(const char* fn);

FILE* loadFile(const char* fn);

FILE* loadFile(const char* fn){
	FILE* fp = NULL;
	if ( ( fp = fopen(fn, "r") ) == NULL ) {
		fprintf(stderr, "Failed to open file.\n");
		return NULL;
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

	if(cityCount == -1){
		fprintf(stderr, "Failed to count cities.\n");
		return NULL;
	}

	FILE* fp = loadFile(fn);

	if(fp == NULL){
		fprintf(stderr, "Failed to open file.\n");
		return NULL;
	}

	struct Graph* g = createGraph((unsigned)cityCount);
	if(g == NULL){
		fprintf(stderr, "Failed to create graph.\n");
		return NULL;
	}

	struct hashTable* table = createTable((unsigned)cityCount);
	if(g == NULL){
		fprintf(stderr, "Failed to create hashTable.\n");
		destroyGraph(g);
		return NULL;
	}


	while(fgets(line, sizeof(line), fp) != NULL){
		//if sscanf doesn't get all three variables, 
        if(sscanf(line, "%s\t%s\t%d[^\n]", city1, city2, &distance) != 3){
			fprintf(stderr, "Energy file not formatted correctly.\n");
			destroyGraph(g);
			destroyTable(table);
			return NULL;
		}

        if(findEntry(table, genHash(city1)) == -1){

            if(addNode(g, (unsigned)addEntry(table, city1))){
				fprintf(stderr, "Adding node to %s graph failed.\n", city1);
				destroyGraph(g);
				destroyTable(table);
				return NULL;
			}
        }
        if(findEntry(table, genHash(city2)) == -1){

            if(addNode(g, (unsigned)addEntry(table, city2))){
				fprintf(stderr, "Adding node %s to graph failed.\n", city2);
				destroyGraph(g);
				destroyTable(table);
				return NULL;
			}
        }
        if(addEdge(g, (unsigned)findEntry(table, genHash(city1)), 
        (unsigned)findEntry(table, genHash(city2)), distance)){
			fprintf(stderr, "Adding edge (%s to %s) to graph failed.\n", city1, city2);
			destroyGraph(g);
			destroyTable(table);
			return NULL;
		}
    }
	fclose(fp);

	struct mapStruct* ms = malloc(sizeof(struct mapStruct));

	if(ms==NULL){
		fprintf(stderr, "Malloc fail!\n");
		return NULL;
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
	unsigned int cityCount = 0;

	if(fp == NULL){
		fprintf(stderr, "Failed to open file.\n");
		free(cities);
		return -1;
	}

	if(cities==NULL){
		fprintf(stderr, "Malloc fail!\n");
		free(cities);
		fclose(fp);
		return -1;
	}

    while(fgets(line, sizeof(line), fp) != NULL){
		c1a = 1;
		c2a = 1;
        if(sscanf(line, "%s\t%s[^\n]", city1, city2) != 2){
			fprintf(stderr, "Invalid citypair file!\n");
			fclose(fp);
			free(cities);
			return -1;
		}

		c1h = genHash(city1); //use the hash function from the hashtable
		c2h = genHash(city2); //so we dont need to compare strings

		for(unsigned int i=0;i<cityCount;i++)
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
	return (int)cityCount;

}

unsigned int loadPairs(struct Graph* g, struct hashTable* ht, const char* fn)
{
	FILE* fp = loadFile(fn);

	if(fp == NULL){
		fprintf(stderr, "Failed to open file.\n");
		return 1;
	}

	//temporary buffers
	char line[70];
	char city1[15];
	char city2[15];

	while(fgets(line, sizeof(line), fp) != NULL){
		if(sscanf(line, "%s\t%s[^\n]", city1, city2) != 2){
			fclose(fp);
			fprintf(stderr, "Invalid citypairs file.\n");
			return 1;
		}
		if(findShortestPath(g, ht, city1, city2) == 1){
			fclose(fp);
			fprintf(stderr, "Pathfinder failed.\n");
			return 1;
		}
	}
	fclose(fp);
	return 0;

}
