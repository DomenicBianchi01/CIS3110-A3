#ifndef __DBIANCHI_PARSEFILE__
#define __DBIANCHI_PARSEFILE__

struct Process {

	char ID;
	int memory;
	int numOfSwaps;
	int time;
	int startIndex;
	int endIndex;

	struct Process * next;
};
typedef struct Process Process;

/**
 * parseDataFile
 * Gets data from a text file and creates processes
 * IN: char fileName[]
 * OUT: Process
 * POST: A list of processes is returned
 * ERROR: If the text file is not formatted correctly, an error will occur
 **/
Process * parseDataFile(char fileName[]);

/**
 * createProcess
 * Allocates memory for the process
 * IN: char IDChar, int memory, int swaps, int time, int start, int end
 * OUT: Process
 * POST: A process node is returned
 * ERROR: If malloc fails, the process will not be created
 **/
Process * createProcess(char IDChar, int memory, int swaps, int time, int start, int end);

/**
 * addToBack
 * Add the process to the back of the linked list
 * IN: Process * theList, Process * toBeAdded
 * OUT: Process
 * POST: A list of processes is returned
 * ERROR: If malloc fails, the process will not be created
 **/
Process * addToBack(Process * theList, Process * toBeAdded);

/**
 * addToBack
 * Add the process to a linked list of processes currently in memory
 * IN: Process * list, Process * node, int clock, int start, int end
 * OUT: Process
 * POST: A list of processes is returned
 * ERROR: If malloc fails, the process will not be created
 **/
Process * insertProcess(Process * list, Process * node, int clock, int start, int end);

/**
 * loadProcess
 * Pops the process off the "waiting" queue and calls insertProcess(). Also cals getStats() which calculates statistics about the current state of the simulation
 * IN: Process ** memoryNodes, Process ** queue, int memory[], double * processCounter, double * holeCounter, int * numOfLoads, int i, int startIndex, int clock
 * OUT: NONE
 * POST: Process is loaded into memory
 * ERROR: If the queue is empty for some reason at this point, a fatal error will occur
 **/
void loadProcess(Process ** memoryNodes, Process ** queue, int memory[], double * processCounter, double * cumulativeMem, double * holeCounter, int * numOfLoads, int i, int startIndex, int clock);

/**
 * removeProcess
 * Removes the process from memory
 * IN: Process ** memoryNodes, Process ** queue, int memory[]
 * OUT: NONE
 * POST: Process is removed from memory. If the process has already been swapped three times, it is not added back to the "waiting" queue
 * ERROR: If the queue is empty for some reason at this point, a fatal error will occur
 **/
void removeProcess(Process ** memoryNodes, Process ** queue, int memory[]);

/**
 * getStats
 * Calculates stats regarding the current state of the program
 * IN: rocess * list, int memoryArray[], int * numOfProcesses, int * numOfHoles, double * memusage, int * cumulativeMem
 * OUT: NONE
 * POST: Process is removed from memory. If the process has already been swapped three times, it is not added back to the "waiting" queue
 * ERROR: If the queue is empty for some reason at this point, a fatal error will occur
 **/
void getStats(Process * list, int memoryArray[], int * numOfProcesses, int * numOfHoles, double * memusage);

/**
 * clearArray
 * Clears all the elements of a char array so the array can be re-used for other purposes elsewhere in the program
 * IN: char string[], int length
 * OUT: None
 * POST: Empties character array
 * ERROR: If the length variable (int length) is shorter than the actually length of the char array, not all elements of the array will be cleared which will result in errors when the array is re-used else where in the program
 **/
void clearArray(char string[], int length);

/**
 * setArray
 * Sets all the elements of a int array to 0 so the array can be re-used for other purposes elsewhere in the program
 * IN: char array[], int length
 * OUT: None
 * POST: Sets all elements to 0
 * ERROR: If the length variable (int length) is shorter than the actually length of the char array, not all elements of the array will be set to zero which may result in errors when the array is re-used else where in the program
 **/
void setArray(int array[], int length);

/**
 * destroyList
 * Free all process that are still in memory at the end of the simulation
 * IN: Process * list
 * OUT: None
 * POST: All processes are free.
 * ERROR: No error possible
 **/
void destroyList(Process * theList);

/**
 * simulation
 * Main loop of the entire program. Performs 4 memory managment algorithms
 * IN: Process * queue, int algo
 * OUT: None
 * POST: One of the 4 memory managment algorithms is complete
 * ERROR: See other function headers above
 **/
void simulation(Process * queue, int algo);


#endif
