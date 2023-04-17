#include "error.h"
#include <stdlib.h>
#include <stdio.h>

void errorHandler(const char* msg){
	fprintf(stderr, "\n%s\nExiting due to error..\n", msg);
	exit(1);
}