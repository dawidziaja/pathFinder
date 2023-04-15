#ifndef _GRAPH_H
#define _GRAPH_H
#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>

struct Graph;

struct Node;

struct Edge;

struct Graph* createGraph(int);

void destroyGraph(struct Graph*);


int addEdge(struct Graph*, int, int, int);


void addNode(struct Graph*, int);


int getEdgeWeight(struct Edge*);

int getEdgeDestination(struct Edge*);

struct Edge** returnEdgeList(struct Node *);

struct Node* returnNode(struct Graph* g, int id);

void printGraph(struct Graph*, struct tableEntry**);

int getNodeCount(struct Graph*);

int getEdgeCount(struct Node*);

int getTotalEdges(struct Graph*);

int getNodeID(struct Node* n);

int getConnectionWeight(struct Graph* g, int src, int dst);

void setEdgeWeight(struct Edge*, int weight);

#endif /* _GRAPH_H */
