#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

struct ThreadData {
long ***A, ***B;
long *currentSizeAA, *currentSizeBB;
int **currentSizeA, **currentSizeB;
int myRole;
};


void error(int errorID) {
	if (errorID == 1)
		printf("Error: Too many stars, please limit seperators to 8 max");
	else
		printf("Error: creating pthreads. Return code: %d", errorID);
	exit(0);
}

void *matrixMultiply(void *param) {
	struct ThreadData* data = (struct ThreadData*) param;

return NULL;
}

int main() {
	//Input
	long **A, **B;
	int currentSizeAA = 0;
	A = (long **) malloc(sizeof(long *));

	//Matrix A
	int *currentSizeA = malloc(sizeof(int));	
	char tempString[256];
	while (1) {
		scanf(" %[^\n]", tempString);	
		char *pEnd = &tempString[0];
		long tempC = strtol(pEnd, &pEnd, 10);
		do {
		if (*pEnd == '*') {
		//Asserts for 8 or less stars
			int counter = 0;
			while (*(pEnd+(counter*sizeof(char))) == '*') ++counter;
			if (counter > 8) error(1);
			goto MatrixB;
		}
		else {
			A[currentSizeAA] = (long *) realloc(A[currentSizeAA], ++currentSizeA[currentSizeAA]*sizeof(long));
			A[currentSizeAA][currentSizeA[currentSizeAA]-1] = tempC;
		}
		}
		while ((tempC = strtol(pEnd, &pEnd, 10)) != 0);	
		A = (long **) realloc(A,++currentSizeAA*sizeof(long *));
		currentSizeA = realloc(currentSizeA, currentSizeAA*sizeof(int));
	}

MatrixB:

	;//Matrix B
	int currentSizeBB = 0;
	int *currentSizeB = malloc(sizeof(int));
	B = (long **) malloc(sizeof(long *));
	memset(&tempString[0], 0, sizeof(tempString));
	while (scanf(" %[^\n]",tempString) == 1) {
		if ('\n' == tempString[0]) break;	
		char *pEnd = &tempString[0];
		long tempC = strtol(pEnd, &pEnd, 10);
		do {
			B[currentSizeBB] = (long *) realloc(B[currentSizeBB], ++currentSizeB[currentSizeBB]*sizeof(long));
			B[currentSizeBB][currentSizeB[currentSizeBB]] = tempC;
	  	}
		while ((tempC = strtol(pEnd, &pEnd, 10)) != 0);	
		B = (long **) realloc(B,++currentSizeBB*sizeof(long *));
		currentSizeB = realloc(currentSizeB, currentSizeBB*sizeof(int));
	}
	
	//Consider checking for valid matrix multiplication (or leave it as a constraint)

	//Calling threads for matrix multiplcation
	int maxB = currentSizeB[0];
	for (int i = 1; i < currentSizeBB-1; i++) maxB = maxB + ((currentSizeB[i] - maxB) & ((currentSizeB[i] - maxB) >> (sizeof(int) * 8 - 1)));
	pthread_t threads[currentSizeBB]; //Rows of Matrix B (ASSUMPTION: Rows of B = length of columns for each Ai)
	struct ThreadData data[currentSizeBB];
	static long **C;
	C =  malloc(sizeof(long *)*currentSizeAA);
	
	for(int i = 0; i < currentSizeBB; i++) {
		data[i].A=&A;
		data[i].B=&B;
		data[i].currentSizeAA = &currentSizeAA;
		data[i].currentSizeBB = &currentSizeBB;
		data[i].currentSizeA = &currentSizeA;
		data[i].currentSizeB = &currentSizeB;
		data[i].myRole = i;
	}

	for (int i = 0; i < currentSizeBB; i++) {
		int rc = pthread_create(&threads[i], NULL, matrixMultiply, &data[i]);
		if (rc) error(rc);
	}	 

	for (int i = 0; i < currentSizeBB; i++)
		pthread_join(threads[i], NULL);

	//Output
	for (int i = 0; i <= currentSizeAA-1; i++) { 
		for (int k = 0; k < currentSizeA[i]; k++) 
			printf("%ld\t", A[i][k]);
			printf("\n");
		}

	free(A);
	free(B);
	return 0;
}

