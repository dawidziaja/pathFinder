#include "graph.h"

struct Edge {
    int destination;
    int weight;
};
struct Node {
    int id;                /*id of node in hash table*/
    int edges;             /*number of edges in the edge list*/
    struct Edge** edgeList;
};
struct Graph {
    int nodeCount;             /* number of nodes */
    int edgeCount;              /* number of edges */
    struct Node** nodes;   /* pointer to array of nodes*/
};
struct Graph* createGraph(int nodeCount){
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->nodes=calloc(nodeCount, sizeof(struct Node*));
    graph->nodeCount = 0;
    graph->edgeCount = 0;
    return(graph);
}

void destroyGraph(struct Graph * graph)
{
    int i;
    int j;

    struct Node * node = NULL;

    for(i=0;i<graph->nodeCount;i++)
    {
        node = ( graph->nodes [ i ] );

        for(j=0;j<node->edges;j++)

            free(node->edgeList[j]);
        

        free(node->edgeList);
        free(node);
    }
    free(graph->nodes);
    free(graph);
}
void addNode( struct Graph* g, int id )
{
    if( g->nodes[id] == NULL ){
        g->nodes[id] = malloc( sizeof( struct Node ) );
        g->nodes[id]->id = id;
        g->nodeCount++;
        g->nodes[id]->edges = 0;
        g->nodes[id]->edgeList = NULL;
    }

}

int addEdge(struct Graph* g, int from, int to, int weight)
{
    //increment the number of edges by 2.
    g->edgeCount+=2;
    //get the node objects for source and destination
    struct Node* a = returnNode(g, from);
    struct Node* b = returnNode(g, to);
    //increment the number of edges on each node
    a->edges++;
    b->edges++;
    //realloc the edgelist so we have enough memory
    a->edgeList = realloc(a->edgeList, (sizeof(struct Node*) * a->edges) );
    b->edgeList = realloc(b->edgeList, (sizeof(struct Node*) * b->edges) );

    //allocate the memory for a new edge
    int currentEdgeA = a->edges-1;
    int currentEdgeB = b->edges-1;
    a->edgeList[currentEdgeA] = malloc(sizeof(struct Edge));
    b->edgeList[currentEdgeB] = malloc(sizeof(struct Edge));

    //Create new edge at source and destination
    a->edgeList[currentEdgeA]->destination = to;
    a->edgeList[currentEdgeA]->weight = weight;

    b->edgeList[currentEdgeB]->destination = from;
    b->edgeList[currentEdgeB]->weight = weight;
}

struct Edge** returnEdgeList(struct Node* n)
{
    return n->edgeList;
}

struct Node* returnNode(struct Graph* g, int id)
{
    return g->nodes[id];
}

int getEdgeWeight(struct Edge* e)
{
    return e->weight;
}

int getEdgeDestination(struct Edge* e)
{
    return e->destination;
}

int getNodeCount(struct Graph* g){
    return g->nodeCount;
}

int getEdgeCount(struct Node* n){
    return n->edges;
}

int getNodeID(struct Node* n){
    return n->id;
}

int getTotalEdges(struct Graph* g){
    return g->edgeCount;
}

int getConnectionWeight(struct Graph* g, int src, int dst){
    struct Node* sourceNode = g->nodes[src];
    struct Edge** edgeList = returnEdgeList(sourceNode);
    for(int i=0;i<sourceNode->edges; i++){
        if( edgeList[i]->destination == dst ){
            return edgeList[i]->weight;
        }
    }
}

void setEdgeWeight(struct Edge* e, int weight){
    e->weight = weight;
}

void printGraph(struct Graph* g, struct tableEntry** table)
{
    int i;
    int j;
    struct Node* currentNode = NULL;
    struct Edge* currentEdge = NULL;
    printf("Printing Graph with %d vertices and %d edges\n",g->nodeCount, 
    g->edgeCount);
    for ( i = 0; i < g->nodeCount; i++ ){
        currentNode = g->nodes[i];

        printf("Node id %d (%s), contains the following edges:\n", 
        currentNode->id, getText(table, currentNode->id));

        for( j = 0; j < currentNode->edges; j++){
            currentEdge = currentNode->edgeList[j];
            if(currentEdge != NULL)
                printf("Edge to id %d (%s) with weight %d\n", 
                currentEdge->destination, 
                getText(table, currentEdge->destination), 
                currentEdge->weight);
        }
        printf("\n");
    }
}