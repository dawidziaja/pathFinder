#ifndef _PATHFINDER_H
#define _PATHFINDER_H

#include "graph.h"

#include <string.h>

struct pathList;

void normaliseGraph(struct Graph *);
void bellmanFord(struct Graph * g, int src, int dst);
void recursiveDFS(struct Graph* g, int src, int dst, int* visited, int* currentPath, struct pathList* pl);
void findShortestPath(struct Graph* g, struct tableEntry** table, char* a, char* b);
int* shortestPathInList(struct Graph* g, struct pathList* pl, int a, int b);
void printPath(struct Graph* g, struct tableEntry** table, int* path, int a, int b);
int* allocatePath(int nodeCount);
void addPathToList(int* path, struct pathList* pl, int pathSize);
struct pathList* createPathList();
void resizePathList(struct pathList* pl, int size);
void destroyPathList(struct pathList* pl);

#endif /*_PATHFINDER_H*/