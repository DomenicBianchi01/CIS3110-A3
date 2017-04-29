#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "dine.h"

//Mutex variable
pthread_mutex_t forkLock[10000000];

int numOfTimesToEat;
int philosophers;

void dinning(int numOfPhil, int numOfEating) {

	numOfTimesToEat = numOfEating;
    philosophers = numOfPhil;

	//Create array of threads
    pthread_t people[numOfPhil];

    //Initialize the mutex variable
    for (long i = 0; i <= numOfPhil; i++) {

        pthread_mutex_init(&forkLock[i], NULL);
    }

    //Start running each thread
    for (long i = 0; i < numOfPhil; i++) {

        pthread_create(&people[i], NULL, readyToEat,(void*)i);
    }

	//Make sure all threads have completed before exiting the program.
    for (int j = 0; j < numOfPhil; j++) {

        pthread_join(people[j], NULL);
    }
}

void *readyToEat(void * ID) {

	long id;
    int i = 0;
    id = (long)ID;

    //This for loop runs for each thread for however many times each philosopher has to eat

    printf("Philosopher %ld is thinking.\n", id+1);

    while (i < numOfTimesToEat) {

        sleep(1);

        int index = 0;

        if (id == 0) {

            index = philosophers-1;
        }
        else {

            index = id-1;
        }

        //Try to lock the left and right forks
        int lock1 = pthread_mutex_lock(&forkLock[id]);
        int lock2 = pthread_mutex_lock(&forkLock[index]);

        //If both forks were sucessfully locked, then begin eating
        if (lock1 == 0 && lock2 == 0) {

            printf("Philosopher %ld is eating.\n", id+1);
            i = i + 1;
            sleep(1);
        }

        printf("Philosopher %ld is thinking.\n", id+1);

        //Drop forks
        pthread_mutex_unlock(&forkLock[id]);
        pthread_mutex_unlock(&forkLock[index]);
    }

    //Terminate the current thread
    pthread_exit(NULL);

    return NULL;
}
