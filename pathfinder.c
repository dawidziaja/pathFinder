#include "pathfinder.h"

struct pathList{
	int** paths;
	int size;
};

void recursiveDFS(struct Graph*, int, int, int*, int*, struct pathList*);
int* shortestPathInList(struct Graph*, struct pathList*, int, int);
void printPath(struct Graph* g, struct tableEntry**, int*, int, int);
int* allocatePath(int);
void addPathToList(int* path, struct pathList*, int);
struct pathList* createPathList();
void resizePathList(struct pathList*, int);
void destroyPathList(struct pathList*);
int getPathWeight(struct Graph*, int*, int, int);


void recursiveDFS(struct Graph* g, int src, int dst, int* visited, 
int* currentPath, struct pathList* pl)
{
	//mark the source node as visited 
	visited[src] = 1;
	//get the source node and edgelist objects
	struct Node* currentNode = returnNode(g, src);
	struct Edge** edgeList = returnEdgeList(currentNode);
	int destination;
	//iterate through each edge connected to our source node
	for(int i=0; i < getEdgeCount(currentNode); i++){
		//get the destination of current edge
		destination = getEdgeDestination(edgeList[i]);

		//if our edge leads to the final destination (dst), then we
		//have a valid path
		if(destination == dst){
			//add step to path
			currentPath[destination] = src;
			//add the path to our list
			addPathToList(currentPath, pl, getNodeCount(g));
		}
		else if( !visited[destination] ){
			//add step to path
			currentPath[destination] = src;
			//create a copy of the visited list
			int* visited2 = malloc(sizeof(int)*getNodeCount(g));
			memcpy(visited2, visited, sizeof(int)*getNodeCount(g));
			//run with our copy of the visited list
			recursiveDFS(g, destination, dst, visited2, currentPath, pl);
			//dont leak memory :)
			free(visited2);
		}
	}
}
//Find shortest path from a to b on graph g
void findShortestPath(struct Graph* g, struct tableEntry** table, 
char* a, char* b)
{
	int ida = findEntry(table, genHash(a));
	int idb = findEntry(table, genHash(b));
	//create our path list
	struct pathList* pl = createPathList();

	//allocate some working memory for the dfs
	int* currentPath = malloc(sizeof(int)*getNodeCount(g));
	int* visited =  malloc(sizeof(int)*getNodeCount(g));

	//set initial values
	for(int i=0; i < getNodeCount(g); i++){
        visited[i] = 0;
        currentPath[i] = -1;
    }
	//run dfs
	recursiveDFS(g, ida, idb, visited, currentPath, pl);

	int* shortestPath = shortestPathInList(g, pl, ida, idb);

	printPath(g, table, shortestPath, ida, idb);
	//free our pointers :)
	free(currentPath);
	free(visited);
	destroyPathList(pl);
}

int* shortestPathInList(struct Graph* g, struct pathList* pl, int a, int b)
{
	int* weights = malloc(sizeof(int)*pl->size);
	//iterate through every path
	for(int i=0;i < pl->size; i++)
	{
		//store path to be worked on
		int* path = pl->paths[i];
		//printPath(g, path, a,b);
		//get predecessor
		int predecessor = path[b];
		int pathWeight = getConnectionWeight(g, predecessor, b);
		//trace the path until we get back to our starting node
		while(predecessor != a){
			pathWeight+= getConnectionWeight(g, predecessor, path[predecessor]);
			predecessor = path[predecessor];
		}
		weights[i] = pathWeight;
	}
	//find the smallest path
	int smallest = 0;
	for(int i=0;i< pl->size; i++)
	{
		if(weights[i] < weights[smallest])
		{
			smallest = i;
		}
	}
	free(weights);
	return pl->paths[smallest];
}

void printPath(struct Graph* g, struct tableEntry** table, int* path, 
int a, int b)
{
	int predecessor = path[b];

	printf("\nPath from %s to %s : Total W : %d\n", getText(table, a), 
	getText(table, b), getPathWeight(g, path,a,b));

	printf("Conn from %s to %s (Weight: %d)\n", getText(table, b), 
	getText(table, predecessor), 
	getConnectionWeight(g, b, predecessor));

	while(predecessor != a){

		printf("Conn from %s to %s (Weight: %d)\n", getText(table, predecessor), 
		getText(table, path[predecessor]), 
		getConnectionWeight(g, predecessor, path[predecessor]));

		predecessor = path[predecessor];
	}
}

int getPathWeight(struct Graph* g, int* path, int a, int b)
{
	//get predecessor
	int predecessor = path[b];
	int pathWeight = getConnectionWeight(g, predecessor, b);
	//trace the path until we get back to our starting node
	while(predecessor != a){
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

void addPathToList(int* path, struct pathList* pl, int pathSize)
{
	//allocate some new memory for the path, what we pass is used again
	//by the dfs function later
	int* newPath = allocatePath(pathSize);
	//copy over the path to our new bit of memory
	memcpy(newPath, path, pathSize*sizeof(int) );

	//resize our list to be one element larger
	resizePathList(pl, (pl->size)+1 );

	//set the last element in the list to be our new path.
	pl->paths[ (pl->size) - 1 ] = newPath;
}

struct pathList* createPathList(){
	struct pathList* pl = malloc(sizeof(struct pathList));
	pl->paths = NULL;
	pl->size = 0;
	return pl;
}

void resizePathList(struct pathList* pl, int size){
	//check that we aren't shrinking the list
	if(pl->size >= size){
		printf("you fucked up!\n");
		return;
	}

	//realloc the memory for the double pointer
	pl->paths = realloc(pl->paths, sizeof(int*)*size);
	//change our size value
	pl->size = size;
}

void destroyPathList(struct pathList* pl){
	for(int i=0; i < (pl->size); i++){
		free(pl->paths[i]);
	}
	free(pl->paths);
	free(pl);
}