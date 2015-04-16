#include <string.h>
#include <pthread.h>
#include "time_functions.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>


//Global Variables
static int readers = 0;
sem_t mutex, writeOrRead;

void readFunction() {
	sem_wait(&mutex);
	++readers;
	if (readers == 1)
		sem_wait(&writeOrRead);
	sem_post(&mutex);
	--readers;
	if (readers == 0)
		sem_post(&writeOrRead);
	sem_post(&mutex);
	printf("DB value read =: %d by reader number: %d", 0, 0);
}

void writeFunction() {
	sem_wait(&writeOrRead);

	

	sem_post(&writeOrRead);
}


typedef struct data {
	time_t secs;
	unsigned short mils;
} myData;

int main(int argc, char *argv[]) {
	if (argc != 5) {
		if ((argc >= 2) && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) { printf("Usage: wu_p4 [FILE] [Number of Writes] [Number of Reads]...\nReads and writes to a shared file using a monitor. \n\nExample:\twu_p5 10 10 10 10\nSimulates a buffer shared file being accessed by 10 readers/writers with 10 delay for readers/writers\n"); return -1;
		}
		else
		printf("Invalid arguments. Please use flags --help or -h for assistance\n");
		return -1;
	}
	else {
		//Main Program

		//Initialization
		int numOfReads = atoi(argv[1]), numOfWrites = atoi(argv[2]),
				 readDelay = atoi(argv[3]), writeDelay = atoi(argv[4]);
		pthread_t readers[numOfReads], writers[numOfWrites];
		time_t secs = 0;
		unsigned short mils = 0;	
		get_wall_clock(&secs, &mils);
		
		sem_init(&mutex, 0, 1);
		sem_init(&writeOrRead, 0, 1);
	
		//Starts the Readers
		for (int i = 0; i < numOfReads; i++)
			pthread_create(&readers[i], NULL, (void *) readFunction, NULL);		
		//Starts the Writers
		for (int i = 0; i < numOfWrites; i++)
			pthread_create(&writers[i], NULL, (void *) writeFunction, NULL);

		//Waiting on the threads
		for (int i = 0;  i < numOfReads; i++)
			pthread_join(readers[i], NULL);
		for (int i = 0; i < numOfWrites; i++)
			pthread_join(writers[i], NULL);

		//Output	
	}
	return 0;
}
