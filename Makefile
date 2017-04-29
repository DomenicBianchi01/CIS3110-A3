all: programs

programs:
	gcc -Wall -std=c99 -Iinclude src/holes.c src/parseFile.c -o holes
	gcc -Wall -std=c99 -Iinclude src/dine.c src/parseFile2.c -o dine -lpthread