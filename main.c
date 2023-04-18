#include "hashtable.h"
#include "graph.h"
#include "pathfinder.h"
#include "io.c"

int main(int argc, char *argv[]){

    char* arg1;
    char* arg2;

    if(argc == 3){
        arg1 = argv[1];
        arg2 = argv[2];
    }else{
        //DEFAULT VALUES
        arg1 = "energy-v23-1.txt";
        arg2 = "citypairs.txt";
    }
    struct mapStruct* ms = loadEnergy(arg1);
    if(ms==NULL){
        fprintf(stderr, "Failed to load energy.\n");
        free(ms);
        return EXIT_FAILURE;
    }
    struct Graph* g = ms->graph;
    struct hashTable* table = ms->table;
    if(loadPairs(g, table, arg2) == 1){
        fprintf(stderr, "Failed to load city pairs.\n");
        destroyGraph(g);
        destroyTable(table);
        free(ms);
        return EXIT_FAILURE;
    }
    free(ms);
    destroyGraph(g);
    destroyTable(table);
    return EXIT_SUCCESS;

}
