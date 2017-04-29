#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parseFile.h"

Process * parseDataFile(char fileName[]) {

	FILE * fp = NULL;
	Process * processList = NULL;
	char input[256];
	char memory[256];
	int counter = 0;

	clearArray(input, 256);
	clearArray(memory, 256);

	fp = fopen(fileName, "r");

	//Read in each line of the file
	while (fgets(input, 256, fp) != NULL) {

		Process * nodeToAdd = NULL;

		//Memory values are all characters after the second character in each line
		for (int i = 2; i < strlen(input); i++) {

			memory[counter] = input[i];
			counter = counter + 1;
		}

		//Create a process and add it to the end of the queue
		nodeToAdd = createProcess(input[0], atoi(memory), 0, 0, 0, 0);
		processList = addToBack(processList, nodeToAdd);

		clearArray(memory, 256);
		counter = 0;
	}

	fclose(fp);

	return processList;
}

void getStats(Process * list, int memoryArray[], int * numOfProcesses, int * numOfHoles, double * memusage) {

	bool isHole = false;
	double memCount = 0;

	//Calculate the number of processes currently in memory
	while (list != NULL) {

		*numOfProcesses = *numOfProcesses + 1;

		list = list->next;
	}

	//Count number of holes and total memory usage
	for (int i = 0; i < 128; i++) {

		//Hole count
		if (memoryArray[i] == 0 && isHole == false) {

			isHole = true;
			*numOfHoles = *numOfHoles + 1;
		}
		//Memory count
		else if (memoryArray[i] == 1) {

			isHole = false;
			memCount = memCount + 1;
		}
	}

	//Determine memory util
	*memusage = (memCount/128) * 100;
}

void loadProcess(Process ** memoryNodes, Process ** queue, int memory[], double * processCounter, double * holeCounter, double * cumulativeMem, int * numOfLoads, int i, int startIndex, int clock) {

	int numOfProcesses = 0;
	int numOfHoles = 0;
	double memusage = 0;

	//Pop process off the queue
	Process * node = *queue;
	*queue = (*queue)->next;

	//Insert process into the list of processes in memory
	*memoryNodes = insertProcess(*memoryNodes, node, clock, startIndex, i);
	*numOfLoads = *numOfLoads + 1;

	//Mark which bytes are occupied
	for (int j = startIndex; j <= i; j++) {

		memory[j] = 1;
	}

	//Determine stats for the newly loaded process
	getStats(*memoryNodes, memory, &numOfProcesses, &numOfHoles, &memusage);

	*processCounter = *processCounter + numOfProcesses;
	*holeCounter = *holeCounter + numOfHoles;
	*cumulativeMem = *cumulativeMem + memusage;

	printf("%c loaded, #processes = %d, #holes = %d, memusage = %lf, cumulative mem = %lf\n", node->ID, numOfProcesses, numOfHoles, memusage, *cumulativeMem / *numOfLoads);
}

void removeProcess(Process ** memoryNodes, Process ** queue, int memory[]) {

	//Pop process off
	Process * nodeToRemove = *memoryNodes;
	*memoryNodes = (*memoryNodes)->next;

	//Mark memory that used to be occupied as now free
	for (int z = nodeToRemove->startIndex; z <= nodeToRemove->endIndex; z++) {

		memory[z] = 0;
	}

	//If the process has already been swapped 3 times do not re-add it to the queue of processes waiting to go into memory
	if (nodeToRemove->numOfSwaps != 2) {

		nodeToRemove->numOfSwaps = nodeToRemove->numOfSwaps + 1;
		nodeToRemove->next = NULL;
		*queue = addToBack(*queue, nodeToRemove);
	}
	else {

		free(nodeToRemove);
	}
}

void simulation(Process * queue, int algo) {

	int memory[128];
	int clock = 0;
	int numOfLoads = 0;
	double processCounter = 0;
	double holeCounter = 0;
	double cumulativeMem = 0;

	Process * memoryNodes = NULL;

	setArray(memory, 128);

	//First Fit
	if (algo == 1) {

		printf("----------\nFirst Fit:\n----------\n");

		//Simulate until all remaining processes are in memory
		while (queue != NULL) {

			int space = 0;
			int startIndex = -1;
			bool noSpace = true;

			for (int i = 0; i < 128; i++) {

				if (memory[i] == 0) {

					//If this is the first free byte in the current hole, save its index position
					if (startIndex == -1) {

						startIndex = i;
					}

					space = space + 1;

					//If there is enough room to add the next process, add it to memory
					if (space >= queue->memory) {

						loadProcess(&memoryNodes, &queue, memory, &processCounter, &holeCounter, &cumulativeMem, &numOfLoads, i, startIndex, clock);

						noSpace = false;
						break;
					}
				}
				else {

					space = 0;
					startIndex = -1;
				}
			}

			//If there is no space to add the next process, remove the oldest process currently in memory
			if (noSpace == true) {

				removeProcess(&memoryNodes, &queue, memory);
			}

			clock = clock + 1;
		}
	}

	//Best Fit
	else if (algo == 2) {

		printf("---------\nBest Fit:\n---------\n");

		//Simulate until all remaining processes are in memory
		while (queue != NULL) {

			int space = 0;
			int smallestSpace = 200;
			int startIndex = 0;
			int startIndex2 = 0;
			int endIndex = 0;
			bool noSpace = true;

			for (int i = 0; i < 128; i++) {

				//If this is the first free byte in the current hole and we are not at the last byte of memory, save its index position
				if (memory[i] == 0 && i != 127) {

					if (startIndex == -1) {

						startIndex = i;
					}

					space = space + 1;
				}
				//Check if we can add the process in the memory that has already been "scanned"
				else {

					if (memory[i] == 0 && i == 127) {

						space = space + 1;
					}

					//If the open hole is the smallest hole we have found, save the starting index position
					if (space < smallestSpace && space >= queue->memory) {
						smallestSpace = space;
						noSpace = false;
						startIndex2 = startIndex;
					}

					space = 0;
					startIndex = -1;
				}
			}

			//If there is no space to add the next process, remove the oldest process currently in memory
			if (noSpace == true) {

				removeProcess(&memoryNodes, &queue, memory);
			}
			//Add the process to memory
			else {

				endIndex = startIndex2 + queue->memory-1;

				loadProcess(&memoryNodes, &queue, memory, &processCounter, &holeCounter, &cumulativeMem,  &numOfLoads, endIndex, startIndex2, clock);

				noSpace = true;
			}

			clock = clock + 1;
		}
	}
	//Worst Fit
	else if (algo == 3) {

		printf("----------\nWorst Fit:\n----------\n");

		//Simulate until all remaining processes are in memory
		while (queue != NULL) {

			int space = 0;
			int smallestSpace = -200;
			int startIndex = 0;
			int startIndex2 = 0;
			int endIndex = 0;
			bool noSpace = true;

			for (int i = 0; i < 128; i++) {

				//If this is the first free byte in the current hole and we are not at the last byte of memory, save its index position
				if (memory[i] == 0 && i != 127) {

					if (startIndex == -1) {

						startIndex = i;
					}

					space = space + 1;
				}
				//Check if we can add the process in the memory that has already been "scanned"
				else {

					if (memory[i] == 0 && i == 127) {

						space = space + 1;
					}

					//If the open hole is the largest hole we have found, save the starting index position
					if (space > smallestSpace && space >= queue->memory) {
						smallestSpace = space;
						noSpace = false;
						startIndex2 = startIndex;
					}

					space = 0;
					startIndex = -1;
				}
			}

			//If there is no space to add the next process, remove the oldest process currently in memory
			if (noSpace == true) {

				removeProcess(&memoryNodes, &queue, memory);
			}
			//Add the process to memory
			else {

				endIndex = startIndex2 + queue->memory-1;

				loadProcess(&memoryNodes, &queue, memory, &processCounter, &holeCounter, &cumulativeMem,  &numOfLoads, endIndex, startIndex2, clock);

				noSpace = true;
			}

			clock = clock + 1;
		}
	}
	//Next fit
	else if (algo == 4) {

		printf("---------\nNext Fit:\n---------\n");

		//Simulate until all remaining processes are in memory
		while (queue != NULL) {

			int space = 0;
			int startIndex = -1;
			int lastIndex = 0;
			bool noSpace = true;
			bool oneLoop = true;

			for (int i = lastIndex; i < 128; i++) {

				if (memory[i] == 0) {

					//If this is the first free byte in the current hole, save its index position
					if (startIndex == -1) {

						startIndex = i;
					}

					space = space + 1;

					//If there is enough room to add the next process, add it to memory
					if (space >= queue->memory) {

						loadProcess(&memoryNodes, &queue, memory, &processCounter, &holeCounter, &cumulativeMem, &numOfLoads, i, startIndex, clock);

						noSpace = false;
						//Save the end index of the last process added
						lastIndex = i;
						break;
					}
				}
				else {

					space = 0;
					startIndex = -1;
				}

				//Loop back to the beginning of memory to check for empty holes
				if (i == 127 && noSpace == true && oneLoop == true) {

					i = 0;
					startIndex = -1;
					space = 0;
					oneLoop = false;
				}
			}

			//If there is no space to add the next process, remove the oldest process currently in memory
			if (noSpace == true) {

				removeProcess(&memoryNodes, &queue, memory);
			}

			clock = clock + 1;
		}
	}

	processCounter = processCounter/numOfLoads;
	holeCounter = holeCounter/numOfLoads;

	printf("Total loads = %d, average #processes = %lf, average #holes = %lf, cumulative mem = %lf\n", numOfLoads, processCounter, holeCounter, cumulativeMem/numOfLoads);

	//Free all processes that are still in memory before exiting the program
	if (memoryNodes != NULL) {

		destroyList(memoryNodes);
	}
}

Process * insertProcess(Process * list, Process * node, int clock, int start, int end) {

	node->time = clock;
	node->startIndex = start;
	node->endIndex = end;
	node->next = NULL;

	list = addToBack(list, node);

	return list;
}

Process * createProcess(char IDChar, int memory, int swaps, int time, int start, int end) {

	Process * current;

	//Malloc memory for the Process node
	current = malloc(sizeof(Process));

	//If the malloc fails, do not create the node
	if (current == NULL)  {

		free(current);
		return NULL;
	}

	current->ID = IDChar;
	current->memory = memory;
	current->numOfSwaps = swaps;
	current->time = time;
	current->startIndex = start;
	current->endIndex = end;
	current->next = NULL;

	return current;
}

Process * addToBack(Process * theList, Process * toBeAdded) {

	//If no list exists
	if (theList == NULL) {

		return toBeAdded;
	}
	//Go to the end of the list
	else if (theList->next != NULL) {

		addToBack(theList->next, toBeAdded);
	}
	//Add to the back of the list
	else {

		theList->next = toBeAdded;
	}

	return theList;
}

void clearArray(char string[], int length) {

	//Set entire array to null terminators
	for (int j = 0; j < length; j++) {

		string[j] = '\0';
	}
}

void setArray(int array[], int length) {

	//Set entire array to null terminators
	for (int j = 0; j < length; j++) {

		array[j] = 0;
	}
}

void destroyList(Process * theList) {

	//Go to the very end of the list, then start freeing the nodes
	if (theList->next != NULL) {

		destroyList(theList->next);
	}

	free(theList);
}
