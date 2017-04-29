/**********************
 Assignment #3
 Domenic Bianchi
 0921557
 March 27, 2017
 This program simulates four algroithms that manage memory (first fit, best fit, worst fit, next fit).
 ************************/

#include <stdio.h>
#include "parseFile.h"

int main(int argc, char * argv[]) {

	Process * head = NULL;

	//First fit
	head = parseDataFile(argv[1]);
	simulation(head, 1);
	//Best fit
	head = parseDataFile(argv[1]);
	simulation(head, 2);
	//Worst fit
	head = parseDataFile(argv[1]);
	simulation(head, 3);
	//Next fit
	head = parseDataFile(argv[1]);
	simulation(head, 4);

	return 0;
}
