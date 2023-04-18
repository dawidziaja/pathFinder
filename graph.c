#include "graph.h"
#include "limits.h"

struct Edge {
    unsigned int destination;
    int weight;
};
struct Node {
    unsigned int id;                /*id of node in hash table*/
    unsigned int edges;             /*number of edges in the edge list*/
    struct Edge** edgeList;
};
struct Graph {
    unsigned int nodeCount;             /* number of nodes */
    unsigned int edgeCount;              /* number of edges */
    struct Node** nodes;   /* pointer to array of nodes*/
};
struct Graph* createGraph(unsigned int nodeCount){
    struct Graph* graph = malloc(sizeof(struct Graph));

    graph->nodes=malloc(nodeCount * sizeof(struct Node*));
    for(unsigned int i=0;i<nodeCount;i++){
        graph->nodes[i] = NULL;
    }

    //our second malloc failed so we return NULL.
    if(graph->nodes == NULL){
        fprintf(stderr, "Failed to allocate memory for graph.\n");
        free(graph);
		return(NULL);
	}

    graph->nodeCount = 0;
    graph->edgeCount = 0;
    return(graph);
}

void destroyGraph(struct Graph * graph)
{

    struct Node * node = NULL;

    //Iterate through each node
    for(unsigned int i=0;i<graph->nodeCount;i++)
    {
        node = ( graph->nodes [ i ] );
        if(node != NULL){

        //free each edge in the node's edgelist
        for(unsigned int j=0;j<node->edges;j++)

            free(node->edgeList[j]);
        
        //free the edgeList
        free(node->edgeList);
        //free the node
        free(node);
        }
    }
    //free the nodelist
    free(graph->nodes);
    //free the graph
    free(graph);
}

unsigned int addNode( struct Graph* g, unsigned int id )
{
    if( g->nodes[id] == NULL ){
        g->nodes[id] = malloc( sizeof( struct Node ) );

        if(g->nodes[id] == NULL){
		    fprintf(stderr, "Malloc fail!\n");
            return 1;
	    }

        g->nodes[id]->id = id;
        g->nodeCount++;
        g->nodes[id]->edges = 0;
        g->nodes[id]->edgeList = NULL;
        return 0;
    }
    fprintf(stderr, "Node with id %d already exists!\n", id);
    return 1;

}


//This function assumes all edges are bidirectional, therefore it has to be
//added at both the source and destination node.
unsigned int addEdge(struct Graph* g, unsigned int from, unsigned int to, int weight)
{
    //increment the number of edges by 2.
    g->edgeCount+=2;
    //get the node objects for source and destination
    struct Node* a = returnNode(g, from);
    struct Node* b = returnNode(g, to);
    if((a == NULL) || (b == NULL)){
        return 1;
    }
    //increment the number of edges on each node
    a->edges++;
    b->edges++;
    //realloc the edgelist so we have enough memory
    a->edgeList = realloc(a->edgeList, 
    (sizeof(struct Node*) * (unsigned long)a->edges) );
    b->edgeList = realloc(b->edgeList, 
    (sizeof(struct Node*) * (unsigned long)b->edges) );

    if((a->edgeList == NULL) || (b->edgeList == NULL)){
        fprintf(stderr, "Malloc fail!\n");
        //free both pointers in case just one realloc failed.
        free(a->edgeList);
        free(b->edgeList);
		return 1;
	}

    //allocate the memory for a new edge
    unsigned int currentEdgeA = a->edges-1;
    unsigned int currentEdgeB = b->edges-1;
    a->edgeList[currentEdgeA] = malloc(sizeof(struct Edge));
    b->edgeList[currentEdgeB] = malloc(sizeof(struct Edge));

    if((a->edgeList[currentEdgeA] == NULL) || (b->edgeList[currentEdgeB] == NULL)){
        fprintf(stderr, "Malloc fail!\n");
        //free both pointers in case just one realloc failed.
        free(a->edgeList[currentEdgeA]);
        free(b->edgeList[currentEdgeB]);
		return 1;
	}

    //Create new edge at source and destination
    a->edgeList[currentEdgeA]->destination = to;
    a->edgeList[currentEdgeA]->weight = weight;

    b->edgeList[currentEdgeB]->destination = from;
    b->edgeList[currentEdgeB]->weight = weight;

    return 0;
}

struct Edge** returnEdgeList(struct Node* n)
{
    return n->edgeList;
}

struct Node* returnNode(struct Graph* g, unsigned int id)
{
    return g->nodes[id];
}

int getEdgeWeight(struct Edge* e)
{
    return e->weight;
}

unsigned int getEdgeDestination(struct Edge* e)
{
    return e->destination;
}

unsigned int getNodeCount(struct Graph* g){
    return g->nodeCount;
}

unsigned int getEdgeCount(struct Node* n){
    return n->edges;
}

unsigned int getNodeID(struct Node* n){
    return n->id;
}

unsigned int getTotalEdges(struct Graph* g){
    return g->edgeCount;
}

int getConnectionWeight(struct Graph* g, unsigned int src, unsigned int dst){
    struct Node* sourceNode = g->nodes[src];
    struct Edge** edgeList = returnEdgeList(sourceNode);
    //find the edge that connects to the destination
    for(unsigned int i=0;i<sourceNode->edges; i++){
        if( edgeList[i]->destination == dst ){//check if this is our node
            return edgeList[i]->weight; //it is!
        }
    }
    //Connection does not exist - return max weight.
    return INT_MAX;
}

void setEdgeWeight(struct Edge* e, int weight){
    e->weight = weight;
}

void printGraph(struct Graph* g, struct hashTable* table)
{
    struct Node* currentNode = NULL;
    struct Edge* currentEdge = NULL;
    printf("Printing Graph with %d vertices and %d edges\n",g->nodeCount, 
    g->edgeCount);
    for (unsigned int i = 0; i < g->nodeCount; i++ ){
        currentNode = g->nodes[i];

        printf("Node id %d (%s), contains the following edges:\n", 
        currentNode->id, getText(table, currentNode->id));

        for(unsigned int j = 0; j < currentNode->edges; j++){
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
