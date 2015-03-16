#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

struct ThreadData {
long ***A, ***B;
int *currentSizeBB, *currentSizeB;
int x, y;
};

static volatile long **C; //Final new Matrix

void error(int errorID) {
	if (errorID == 0)
		printf("Error: Invalid arguments. Please type --help for more details\n");
	else if (errorID == 1)
		printf("Error: Too many stars, please limit seperators to 8 max\n");
	else
		printf("Error: creating pthreads. Return code: %d\n", errorID);
	exit(0);
}

void *matrixMultiply(void *param) {
	struct ThreadData* data = (struct ThreadData*) param;
	long sum = 0;
	int x = (*data).x % *(*data).currentSizeBB, y = (*data).y % *(*data).currentSizeB;
	for (int i = 0; i < *(*data).currentSizeBB; i++) {
		sum += (*(*data).A)[y][i] * (*(*data).B)[i][x];
	}
	C[(*data).y][(*data).x] = sum; 
return NULL;
}

int main(int argc, char *argv[]) {
int inputOption = (argc == 1) ? 0 : 1;
FILE *fpIn, *fpOut;
if (argc > 3) error(0);
else if (argc >= 2) {
	if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) printf("Usage: wu_p3 [FILE1] [FILE2]...\nComputes two matrices using pthreads. File 1 is input, and File2 is output. \n\nExample:\twu_p3 input.txt output.txt\nBy default of no arguments, user input will be inputted in stdin.");
	else {
		char file[255] = "./";
		strcat(file, argv[1]);
		fpIn = fopen(file, "r");
		if (argc == 3) {
		char file2[255] = "./";
		strcat(file2, argv[2]);
		fpOut = fopen(file2, "w");
		}
	}
}
	//Input
	long **A, **B;
	int currentSizeAA = 0;
	A = (long **) malloc(sizeof(long *));

	//Matrix A
	int *currentSizeA = malloc(sizeof(int));	
	char tempString[256];
	while (1) {
		if (inputOption) { if (fscanf(fpIn, " %[^\n]", tempString) != 1) goto MatrixB; }
		else if (scanf(" %[^\n]", tempString) != 1) goto MatrixB;	
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
	while (1) {
		if (inputOption) {if (fscanf(fpIn, " %[^\n]",tempString) != 1) goto MatrixC; }
		else if (scanf(" %[^\n]", tempString) != 1) goto MatrixC;
		if ('\n' == tempString[0]) break;	
		char *pEnd = &tempString[0];
		long tempC = strtol(pEnd, &pEnd, 10);
		do {
			B[currentSizeBB] = (long *) realloc(B[currentSizeBB], ++currentSizeB[currentSizeBB]*sizeof(long));
			B[currentSizeBB][currentSizeB[currentSizeBB]-1] = tempC;
	  	}
		while ((tempC = strtol(pEnd, &pEnd, 10)) != 0);	
		B = (long **) realloc(B,++currentSizeBB*sizeof(long *));
		currentSizeB = realloc(currentSizeB, currentSizeBB*sizeof(int));
	}
MatrixC:

	;//Consider checking for valid matrix multiplication (or leave it as a constraint)

	//Calling threads for matrix multiplcation
	int maxB = currentSizeB[0];
	for (int i = 1; i < currentSizeBB-1; i++) maxB = maxB + ((currentSizeB[i] - maxB) & ((currentSizeB[i] - maxB) >> (sizeof(int) * 8 - 1)));	
	pthread_t threads[currentSizeAA * maxB]; //Rows of Matrix B (ASSUMPTION: Rows of B = length of columns for each Ai)	
	struct ThreadData data[currentSizeAA * maxB];
	C =  malloc(sizeof(long *)*currentSizeAA * maxB);
	
	for(int i = 0; i < currentSizeAA; i++) {
		for (int j = 0; j < maxB; j++) {
			data[i*maxB+j].A=&A;
			data[i*maxB+j].B=&B;	
			data[i*maxB+j].currentSizeBB = &currentSizeBB;
			data[i*maxB+j].currentSizeB = &currentSizeB[0];
			data[i*maxB+j].x = j;
			data[i*maxB+j].y = i;
		}
		C[i] = malloc(sizeof(long) * maxB);
	}

	for (int i = 0; i < currentSizeAA * maxB; i++) {
		int rc = pthread_create(&threads[i], NULL, matrixMultiply, &data[i]);
		if (rc) error(rc);
	}	 

	for (int i = 0; i < currentSizeAA * maxB; i++)
		pthread_join(threads[i], NULL);



	//Output
	for (int i = 0; i <= currentSizeAA-1; i++) {  
		for (int k = 0; k < currentSizeA[i]; k++) { 
			printf("%ld\t", C[i][k]);
			if (inputOption) fprintf(fpOut, "%ld\t", C[i][k]);
		}
			printf("\n");
			if (inputOption) fprintf(fpOut, "\n");
		} 
	//CleanUp
	free(currentSizeA);
	free(currentSizeB);

	for (int i = 0; i < currentSizeBB; i++)
			free(B[i]);
	for (int i = 0; i < currentSizeAA; i++) {
			free(A[i]);
			free((long *)C[i]);
	}

	free(A);
	free(B);
	free(C);
	return 0;
}

