#include "pathfinder.h"
#include <limits.h>


//needs to be longer than any path
#define INFINITY INT_MAX


static void recursiveDFS(struct Graph*, unsigned int, unsigned int, unsigned int*, 
int*, int*, int*);
static unsigned int printPath(struct Graph* g, struct hashTable*, int*, unsigned int, 
unsigned int);
static int getPathWeight(struct Graph*, int*, unsigned int);

/*this is the graph traversal function. It takes src and dst node ids, an array of
isited nodes and current path, shortest path, and shortest weight. 
travels down all edges of the source node, calling itself recursively until it 
has travelled along every possible path in the graph
It stops every time it reaches the destination node to calculate  the weight of 
the path it has found, compares it against the shortest weight found so far, and 
stores the path it if it is shorter.
Once the algorithm has run through the entire graph, the shortest path from the
initial src and dst will be stored at currentPath*/
static void recursiveDFS(struct Graph* g, unsigned int src, unsigned int dst, 
unsigned int* visited, int* currentPath, int* shortestPath, int* shortestWeight)
{
	//mark the source node as visited 
	visited[src] = 1;
	//get the source node and edgelist objects
	struct Node* currentNode = returnNode(g, src);
	struct Edge** edgeList = returnEdgeList(currentNode);
	unsigned int destination;
	int weight;
	//iterate through each edge connected to our source node
	for(unsigned int i=0; i < getEdgeCount(currentNode); i++){
		//get the destination of current edge
		destination = getEdgeDestination(edgeList[i]);

		//if our edge leads to the final destination (dst), then we
		//have a valid path
		if(destination == dst){
			//add step to path
			currentPath[destination] = (int)src;

			//get weight of our path
			weight = getPathWeight(g, currentPath, destination);
			//if its smaller than the shortest one, we have a winner
			if(weight < *shortestWeight){
				//copy over current path to shortest path
				memcpy(shortestPath, currentPath, 
				sizeof(unsigned int)*getNodeCount(g));
				*shortestWeight = weight;
			}

		}
		else if( !visited[destination] ){
			//add step to path
			currentPath[destination] = (int)src;

			//continue down this branch
			recursiveDFS(g, destination, dst, visited, currentPath, 
			shortestPath, shortestWeight);

			//remove step from path so we can continue down another branch
			visited[destination] = 0;
		}
	}
}
//Find shortest path from a to b on graph g
unsigned int findShortestPath(struct Graph* g, struct hashTable* table, 
char* a, char* b)
{
	int ida = findEntry(table, genHash(a));
	int idb = findEntry(table, genHash(b));
	if((ida == -1) || (idb == -1)){
		fprintf(stderr, "Input nodes don't exist\n");
		return 1;
	}

	//allocate some working memory for the dfs
	int* currentPath = malloc(sizeof(unsigned int)*getNodeCount(g));
	int* shortestPath = malloc(sizeof(unsigned int)*getNodeCount(g));
	unsigned int* visited =  malloc(sizeof(unsigned int)*getNodeCount(g));
	int* shortestWeight = malloc(sizeof(int));
	if( (currentPath == NULL) || (shortestPath == NULL)
	|| (visited == NULL) || (shortestWeight== NULL)){
		fprintf(stderr, "Input nodes don't exist\n");
		return 1;
	}
	*shortestWeight = INFINITY;

	//set initial values
	for(unsigned int i=0; i < getNodeCount(g); i++){
        visited[i] = 0;
        currentPath[i] = -1;
		shortestPath[i] = -1;
    }
	//run dfs
	recursiveDFS(g, (unsigned)idb, (unsigned)ida, visited, currentPath, 
	shortestPath, shortestWeight);


	printPath(g, table, shortestPath, (unsigned)idb, (unsigned)ida);

	//free our pointers :)
	free(currentPath);
	free(visited);
	free(shortestPath);
	free(shortestWeight);
	return 0;
}


static unsigned int printPath(struct Graph* g, struct hashTable* table, int* path, 
unsigned int a, unsigned int b)
{
	//if the destination of the path doesn't have a predecessor, the path
	//does not reach the destination
	if(path[b]==-1){
		fprintf(stderr, "Path is not valid!\n");
		return 1;
	}

	unsigned int predecessor = (unsigned)path[b];

	//print the first connection in the path
	printf("Path from %s to %s : Total W : %d\n", getText(table, b), 
	getText(table,a), getPathWeight(g, path,b));

	printf("%s->%s (Weight: %d)\n", getText(table, b), 
	getText(table, predecessor), 
	getConnectionWeight(g, b, predecessor));

	//follow the path until the end is reached.
	while(predecessor != a){

		printf("%s->%s (Weight: %d)\n", getText(table, predecessor), 
		getText(table, (unsigned)path[predecessor]), 
		getConnectionWeight(g, predecessor, (unsigned)path[predecessor]));

		predecessor = (unsigned)path[predecessor];
	}
	putchar('\n');
	return 0;
}

static int getPathWeight(struct Graph* g, int* path, unsigned int b)
{
	if(path[b]==-1){
		fprintf(stderr, "Path is not valid!\n");
		return INFINITY;
	}
	//get predecessor
	unsigned int predecessor = (unsigned)path[b];
	int pathWeight = getConnectionWeight(g, predecessor, b);
	//trace the path until we get back to our starting node
	while(path[predecessor] != -1){
		pathWeight+= getConnectionWeight(g, predecessor, 
		(unsigned)path[predecessor]);
		predecessor = (unsigned)path[predecessor];
	}
	return pathWeight;
}
