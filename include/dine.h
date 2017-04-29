#ifndef __DBIANCHI_PARSEFILE__
#define __DBIANCHI_PARSEFILE__

/**
 * dinning
 * Creates the threads required for the simulation
 * IN: int numOfPhil, int numOfEating
 * OUT: NONE
 * POST: Simulation complete
 * ERROR: If an error with the pthread library occurs, fatal errors may occur
 **/
void dinning(int numOfPhil, int numOfEating);

/**
 * readyToEat
 * Each thread runs this function and trys to get ownership of the mutex
 * IN: void * ID
 * OUT: NONE
 * POST: Thread eventually got ownership of the mutex and "ate" the required number of times
 * ERROR: If an error with the pthread library occurs, fatal errors may occur
 **/
void *readyToEat(void * ID);

#endif
