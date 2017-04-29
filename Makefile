all: programs

programs:
	gcc -Wall -std=c99 -Iinclude src/holes.c src/runHoles.c -o holes
	gcc -Wall -std=c99 -Iinclude src/dine.c src/runDine.c -o dine -lpthread
