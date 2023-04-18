#include "pathfinder.h"
#include "error.h"


//needs to be longer than any path
#define INFINITY 9999999


void recursiveDFS(struct Graph*, int, int, int*, int*, int*, int*);
void printPath(struct Graph* g, struct hashTable*, int*, int, int);
int* allocatePath(int);
int getPathWeight(struct Graph*, int*, int);


void recursiveDFS(struct Graph* g, int src, int dst, int* visited, 
int* currentPath, int* shortestPath, int* shortestWeight)
{
	//mark the source node as visited 
	visited[src] = 1;
	//get the source node and edgelist objects
	struct Node* currentNode = returnNode(g, src);
	struct Edge** edgeList = returnEdgeList(currentNode);
	int destination;
	int weight;
	//iterate through each edge connected to our source node
	for(int i=0; i < getEdgeCount(currentNode); i++){
		//get the destination of current edge
		destination = getEdgeDestination(edgeList[i]);

		//if our edge leads to the final destination (dst), then we
		//have a valid path
		if(destination == dst){
			//add step to path
			currentPath[destination] = src;

			//get weight of our path
			weight = getPathWeight(g, currentPath, destination);
			//if its smaller than the shortest one, we have a winner
			if(weight < *shortestWeight){
				memcpy(shortestPath, currentPath, sizeof(int)*getNodeCount(g));
				*shortestWeight = weight;
			}
			//add the path to our list
			//addPathToList(currentPath, pl, getNodeCount(g));
		}
		else if( !visited[destination] ){
			//add step to path
			currentPath[destination] = src;

			recursiveDFS(g, destination, dst, visited, currentPath, 
			shortestPath, shortestWeight);

			visited[destination] = 0;
		}
	}
}
//Find shortest path from a to b on graph g
void findShortestPath(struct Graph* g, struct hashTable* table, 
char* a, char* b)
{
	int ida = findEntry(table, genHash(a));
	int idb = findEntry(table, genHash(b));

	//allocate some working memory for the dfs
	int* currentPath = malloc(sizeof(int)*getNodeCount(g));
	int* shortestPath = malloc(sizeof(int)*getNodeCount(g));
	int* visited =  malloc(sizeof(int)*getNodeCount(g));
	int* shortestWeight = malloc(sizeof(int));
	if( (currentPath == NULL) || (shortestPath == NULL)
	|| (visited == NULL) || (shortestWeight== NULL)){
		errorHandler("Malloc fail!");
	}
	*shortestWeight = INFINITY;

	//set initial values
	for(int i=0; i < getNodeCount(g); i++){
        visited[i] = 0;
        currentPath[i] = -1;
		shortestPath[i] = -1;
    }
	//run dfs
	recursiveDFS(g, idb, ida, visited, currentPath, shortestPath, 
	shortestWeight);


	printPath(g, table, shortestPath, idb, ida);

	//free our pointers :)
	free(currentPath);
	free(visited);
	free(shortestPath);
	free(shortestWeight);
}


void printPath(struct Graph* g, struct hashTable* table, int* path, 
int a, int b)
{
	int predecessor = path[b];

	printf("Path from %s to %s : Total W : %d\n", getText(table, b), 
	getText(table,a), getPathWeight(g, path,b));

	printf("%s->%s (Weight: %d)\n", getText(table, b), 
	getText(table, predecessor), 
	getConnectionWeight(g, b, predecessor));

	while(predecessor != a){

		printf("%s->%s (Weight: %d)\n", getText(table, predecessor), 
		getText(table, path[predecessor]), 
		getConnectionWeight(g, predecessor, path[predecessor]));

		predecessor = path[predecessor];
	}
	putchar('\n');
}

int getPathWeight(struct Graph* g, int* path, int b)
{
	//get predecessor
	int predecessor = path[b];
	int pathWeight = getConnectionWeight(g, predecessor, b);
	//trace the path until we get back to our starting node
	while(path[predecessor] != -1){
		pathWeight+= getConnectionWeight(g, predecessor, path[predecessor]);
		predecessor = path[predecessor];
	}
	return pathWeight;
}

int* allocatePath(int nodeCount)
{
	//allocate an array of ints the needed size
	return malloc(sizeof(int)*nodeCount);
}