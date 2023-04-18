#include "hashtable.h"
#include "graph.h"
#include "pathfinder.h"
#include "io.c"

#define MAX_PATHS ( 100 ) 

int main( int argc, char *argv[]){

    struct mapStruct* ms = loadEnergy("energy-v23-1.txt");
    struct Graph* g = ms->graph;
    struct hashTable* table = ms->table;
    free(ms);

    loadPairs(g, table, "citypairs.txt");
    destroyGraph(g);
    destroyTable(table);
}