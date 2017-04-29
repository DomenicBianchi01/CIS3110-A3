/**********************
 Assignment #3
 Domenic Bianchi
 0921557
 March 27, 2017
 This program solves the dining philosophers problem. It is assumed that there is only one fork.
 ************************/

#include <stdio.h>
#include <stdlib.h>
#include "runDine.h"

int main(int argc, char * argv[]) {

	//Check for correct number of parameters
	if (argc != 3) {

		printf("Invalid number of parameters\n");
		return 1;
	}

	//Begin simulation
	dinning(atoi(argv[1]), atoi(argv[2]));

	return 0;
}
