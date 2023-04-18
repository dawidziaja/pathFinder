#ifndef GRAPH_H
#define GRAPH_H
#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>

struct Graph;

struct Node;

struct Edge;

struct Graph* createGraph(unsigned int);

void destroyGraph(struct Graph*);


unsigned int addEdge(struct Graph*, unsigned int, unsigned int, int);


unsigned int addNode(struct Graph*, unsigned int);


int getEdgeWeight(struct Edge*);

unsigned int getEdgeDestination(struct Edge*);

struct Edge** returnEdgeList(struct Node *);

struct Node* returnNode(struct Graph*, unsigned int);

void printGraph(struct Graph*, struct hashTable*);

unsigned int getNodeCount(struct Graph*);

unsigned int getEdgeCount(struct Node*);

unsigned int getTotalEdges(struct Graph*);

unsigned int getNodeID(struct Node* n);

int getConnectionWeight(struct Graph* g, unsigned int src, unsigned int dst);

void setEdgeWeight(struct Edge*, int weight);

#endif /* _GRAPH_H */
