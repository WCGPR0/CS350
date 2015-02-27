#include <stdio.h>
#include "time_functions.h" //This is a header file from Professor Foreman's class
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define MAX_TWEET_COUNT 140


sem_t writeSem, readSem, buffSem;
int currentP = 0;
FILE *fpIn, *fpOut;
char buffer[10][MAX_TWEET_COUNT];

static inline void incrementP() {
	if(++currentP > 10) currentP = 0;
}

void *taskOne() {
	sem_wait(&buffSem);
	sem_wait(&writeSem);
	if ( fgets(buffer[currentP], MAX_TWEET_COUNT, fpIn) == NULL)
		strcpy(buffer[currentP], "\003");
	incrementP();
	sem_post(&writeSem);
	sem_post(&readSem);
	return 0;
}

void *taskTwo(){
	sem_wait(&readSem);
	sem_wait(&writeSem);	
	char tempString[MAX_TWEET_COUNT];
	strcpy(tempString, buffer[currentP]);
	if (strcmp(tempString, "\003") == 0) return 0;
	fprintf(fpOut, tempString);
printf("String wrote:\t%s\n", tempString);
	sem_post(&writeSem);
	sem_post(&buffSem);
	return 0;
}


int main(void) {
	printf("Please provide the name of an input file \n");
	char inputFileName[256]; //Filenames should be limited to 255 characters
	scanf("%s", inputFileName);

	printf("Please provide the name of an output file \n");
	char outputFileName[256]; //Filenames should be limited to 255 characters
	scanf("%s", outputFileName);

	fpIn = fopen(inputFileName, "r");
	fpOut = fopen(outputFileName, "w");
	pthread_t threads[2];	
	sem_init(&writeSem, 0, 1);
	sem_init(&readSem, 0 , 0);
	sem_init(&buffSem, 0, 10);

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


	printf("Wall Clock Time: %10.3f\n", get_wall_clock_diff());
	printf("CPU Time: %10.3f\n", get_CPU_time_diff());

	sem_destroy(&readSem);
	sem_destroy(&writeSem);
	sem_destroy(&buffSem);
	return 0;
}
