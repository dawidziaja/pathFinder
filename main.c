#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "graph.h"
#include "pathfinder.h"

#define MAX_PATHS ( 100 ) 

int main( int argc, char *argv[]){
    FILE *fp = NULL;
    char line[70];
    char city1[15];
    char city2[15];
    int distance;

    struct tableEntry** table = NULL;
    struct Graph* g = createGraph(24);

    if ( ( fp = fopen("energy-v23-1.txt", "r") ) == NULL ) {
        fputs ( "Cannot open data file.\n", stderr );
        return EXIT_FAILURE;
    }

    if ( ( table = createTable() ) == NULL ) {
        fputs ( "Cannot initialise table.\n", stderr );
        return EXIT_FAILURE;
    }

    while(fgets(line, sizeof(line), fp) != NULL){
        sscanf(line, "%s\t%s\t%d[^\n]", city1, city2, &distance);
        if(findEntry(table, genHash(city1)) == -1){
            addNode(g, addEntry(table, city1));
        }
        if(findEntry(table, genHash(city2)) == -1){
            addNode(g, addEntry(table, city2));
        }
        addEdge(g, findEntry(table, genHash(city1)), 
        findEntry(table, genHash(city2)), distance);
    }

    fclose(fp);
    //normaliseGraph(g);
    //bellmanFord(g, 4, 0);
    //recursiveDFS(g, 0, 20, visited, path);
    if(argc!=3){
        printf("incorrect arguments\n");
        return EXIT_FAILURE;
    }
    findShortestPath(g, table, argv[1],argv[2]);
    //printGraph(g, table);
    destroyTable(table);
    destroyGraph(g);
    return EXIT_SUCCESS;
}