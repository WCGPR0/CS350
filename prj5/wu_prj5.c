//Victor Wu
//CS350
//Project 5

#include <string.h>
#include <pthread.h>
#include "time_functions.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>


//Global Variables
static int readers = 0, writers = 0;
sem_t mutex, writeOrRead, order, r, w;

struct data {
	time_t secs;
	unsigned short mils;
	int sleepR, sleepW;
} myData;

void readFunctionOne(void* arg) {
	struct data* my_data = (struct data *) arg;
	sem_wait(&mutex);
	++readers;
	if (readers == 1)
		sem_wait(&writeOrRead);
	sem_post(&mutex);
	--readers;
	if (readers == 0)
		sem_post(&writeOrRead);
	sem_post(&mutex);
	printf(">>> DB value read =: %d:%hu by reader number: %d\n", (int) (*my_data).secs, (*my_data).mils, readers);
	millisleep((*my_data).sleepR);
}

void writeFunctionOne(void* arg) {
	struct data* my_data = (struct data *) arg;
	sem_wait(&writeOrRead);

	get_wall_clock(&(*my_data).secs, &(*my_data).mils);
	printf("*** DB value set to: %d:%hu by writer number: %d\n", (int) (*my_data).secs, (*my_data).mils, 0);		

	sem_post(&writeOrRead);
	millisleep((*my_data).sleepW);
}

void readFunctionTwo(void* arg) {
	struct data* my_data = (struct data *) arg;
	sem_wait(&mutex);
	sem_wait(&r);
	sem_wait(&writeOrRead);
	++readers;
	if (readers == 1)
		sem_wait(&w);
	sem_post(&writeOrRead);
	sem_post(&r);	
	sem_post(&mutex);
	
	printf(">>> DB value read =: %d:%hu by reader number: %d\n", (int) (*my_data).secs, (*my_data).mils, readers);

	sem_wait(&writeOrRead);
	--readers;
	if (readers == 0) sem_post(&w);
	sem_post(&writeOrRead);
	
	millisleep((*my_data).sleepR);
}

void writeFunctionTwo(void* arg) {
	struct data* my_data = (struct data *) arg;
	sem_wait(&order);
	++writers;
	if (writers == 1) sem_wait(&r);
	sem_post(&order);
	sem_wait(&w);

	get_wall_clock(&(*my_data).secs, &(*my_data).mils);
	printf("*** DB value set to: %d:%hu by writer number: %d\n", (int) (*my_data).secs, (*my_data).mils, 0);		

	sem_post(&w);
	sem_wait(&order);
	--writers;
	if (writers == 0) sem_post(&r);
	sem_post(&order);
	millisleep((*my_data).sleepW);
}

void readFunctionThree(void* arg) {
	struct data* my_data = (struct data *) arg;	
	sem_wait(&order);
	sem_wait(&writeOrRead);
	if (readers == 0)
		sem_wait(&mutex);
	++readers;
	sem_post(&order);
	sem_post(&writeOrRead);
	
	printf(">>> DB value read =: %d:%hu by reader number: %d\n", (int) (*my_data).secs, (*my_data).mils, readers);

	sem_wait(&writeOrRead);
	--readers;
	if (readers == 0)
		sem_post(&mutex);
	sem_post(&writeOrRead);

	millisleep((*my_data).sleepR);
}

void writeFunctionThree(void* arg) {
	struct data* my_data = (struct data *) arg;
	sem_wait(&order);
	sem_wait(&mutex);
	sem_post(&order);

	get_wall_clock(&(*my_data).secs, &(*my_data).mils);
	printf("*** DB value set to: %d:%hu by writer number: %d\n", (int) (*my_data).secs, (*my_data).mils, 0);		

	sem_post(&mutex);
	millisleep((*my_data).sleepW);
}


int main(int argc, char *argv[]) {
	if ((argc != 5) && (argc != 6)) {
		if ((argc >= 2) && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) { printf("Usage: wu_p4 [FILE] [Number of Writes] [Number of Reads] [Write Delay] [Read Delay] (option)...\nReads and writes to a shared file using a monitor. Use option 1 for reader priority, 2 for writer priority, and 3 for neither. Defaults to option 2 with no specified input. \n\nExample:\twu_p5 10 10 10 10 2\nSimulates a buffer shared file being accessed by 10 readers/writers with 10 delay for readers/writers\n"); return -1;
		}
		else
		printf("Invalid arguments. Please use flags --help or -h for assistance\n");
		return -1;
	}
	else {
		//Main Program

		//Initialization
		int numOfReads = atoi(argv[1]), numOfWrites = atoi(argv[2]);
		myData.sleepR = atoi(argv[3]);
		myData.sleepW = atoi(argv[4]);
		pthread_t readers[numOfReads], writers[numOfWrites];
		get_wall_clock(&myData.secs, &myData.mils);
		int option = 2;
		if (argc == 6) option = atoi(argv[5]);
		void (*myPtrR)(void *);	
		void (*myPtrW)(void *);
		sem_init(&mutex, 0, 1);
		sem_init(&writeOrRead, 0, 1);
		sem_init(&order, 0, 1);
		sem_init(&r, 0 ,1);
		sem_init(&w, 0 ,1);

		if (option == 1) { myPtrR = &readFunctionOne; myPtrW = &writeFunctionOne; }
		else if (option == 3) {myPtrR = &readFunctionThree; myPtrW = &writeFunctionThree; }
		else {
			myPtrR = &readFunctionTwo;
			myPtrW = &writeFunctionTwo;
		}
		//Starts the Readers
		for (int i = 0; i < numOfReads; i++)
			pthread_create(&readers[i], NULL, (void *) myPtrR, (void *) &myData);		
		//Starts the Writers
		for (int i = 0; i < numOfWrites; i++)
			pthread_create(&writers[i], NULL, (void *) myPtrW, (void *) &myData);

		//Waiting on the threads
		for (int i = 0;  i < numOfReads; i++)
			pthread_join(readers[i], NULL);
		for (int i = 0; i < numOfWrites; i++)
			pthread_join(writers[i], NULL);

		//Output	
	}
	return 0;
}
