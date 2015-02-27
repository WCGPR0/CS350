#include <stdio.h>
#include "time_functions.h" //This is a header file from Professor Foreman's class
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define MAX_TWEET_COUNT 140
#define MAX_DATA 10

//!--- Circular queue array
typedef struct cirQueue {
int first;
int last;
int validItems;
char buffer[MAX_DATA][MAX_TWEET_COUNT];
} cirQueue_t;

int isEmpty(cirQueue_t *myQueue);
int putItem(cirQueue_t *myQueue, char item[MAX_DATA]);
int getItem(cirQueue_t *myQueue, char *item);
//!-- Circular queue array


sem_t writeSem, readSem, buffSem;
int currentP = 0;
FILE *fpIn, *fpOut;
cirQueue_t myQueue = {.first = 0, .last = 0, .validItems = 0, .buffer[0 ... MAX_DATA-1][0 ... MAX_TWEET_COUNT-1] = 0};

void *taskOne() {
	int loop = 1;
	while (loop) {
	sem_wait(&buffSem);
	sem_wait(&writeSem);
	char tempString[MAX_TWEET_COUNT];
	if (fgets(tempString, MAX_TWEET_COUNT, fpIn) == NULL) {
		strcpy(tempString, "\003");
		loop = !loop;
	}
	putItem(&myQueue, tempString);
	sem_post(&writeSem);
	sem_post(&readSem);	
	}
	return NULL;
}

void *taskTwo(){
	while (1) {
	sem_wait(&readSem);
	sem_wait(&writeSem);	
	char tempString[MAX_TWEET_COUNT];
	getItem(&myQueue, tempString);	
	if (strcmp(tempString, "\003") == 0) return NULL;
	fprintf(fpOut, "%s", tempString);
	sem_post(&writeSem);
	sem_post(&buffSem);
printf("String wrote:\t%s\n", tempString);
	}
}


int main(void) {
	printf("Please provide the name of an input file \n");
	char inputFileName[256]; //Filenames should be limited to 255 characters
	scanf("%s", inputFileName);

	printf("Please provide the name of an output file \n");
	char outputFileName[256]; //Filenames should be limited to 255 characters
	scanf("%s", outputFileName);

	//Intialization
	fpIn = fopen(inputFileName, "r");
	fpOut = fopen(outputFileName, "w");
	pthread_t threads[2];	
	sem_init(&writeSem, 0, 1);
	sem_init(&readSem, 0 , 0);
	sem_init(&buffSem, 0, MAX_DATA);	

	start_timing();
	int r1 = pthread_create(&threads[0], NULL, taskOne, NULL);
	int r2 = pthread_create(&threads[1], NULL, taskTwo, NULL);
	if (r1 != 0 || r2 != 0) printf("There was an error creating the pthreads");

	//Blocking until thread completes
	int i = 0;
	for (i = 0; i < 2; i++) {
		int rc = pthread_join(threads[i], NULL);
		if (rc !=0) printf("There was an error with thread %d", i+1);
	}
	stop_timing();

	fclose(fpIn);
	fclose(fpOut);
	printf("Wall Clock Time: %10.3f\n", get_wall_clock_diff());
	printf("CPU Time: %10.3f\n", get_CPU_time_diff());

	sem_destroy(&readSem);
	sem_destroy(&writeSem);
	sem_destroy(&buffSem);
	return 0;
}

//Verifies if the queue is empty (1) or not (0)
int isEmpty(cirQueue_t *myQueue) {
return (myQueue->validItems == 0) ? 1 : 0;
}

//Puts an item into the queue, returns -1 if there exists no space
int putItem(cirQueue_t *myQueue, char item[MAX_TWEET_COUNT]) {
if (myQueue->validItems >= MAX_DATA) return -1;
else {
	myQueue->validItems++;
	strcpy(myQueue->buffer[myQueue->last], item);
	myQueue->last=(myQueue->last+1)%MAX_DATA;
	return 0;
}
}

//Pop an item, returns -1 if queue is empty
int getItem(cirQueue_t *myQueue, char *item) {
	if(isEmpty(myQueue)) return -1;
	else {
	strncpy(item, myQueue->buffer[myQueue->first], MAX_TWEET_COUNT);
	myQueue->first=(myQueue->first+1)%MAX_DATA;
	myQueue->validItems--;
	return 0;
	}
}
