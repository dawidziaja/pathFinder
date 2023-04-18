build:
		clang -o main io.c main.c graph.c hashtable.c pathfinder.c -g -gdwarf-4 -Weverything -Wno-declaration-after-statement -std=c99 -Wno-padded
clean:
		rm -rf main
