#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void error(int errorID) {
	if (errorID == 1) {
		printf("Error: Too many stars, please limit seperators to 8 max");
	}
	exit(0);
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
printf("%ld\n", tempC);	
			A[currentSizeAA] = (long *) realloc(A[currentSizeAA], ++currentSizeA[currentSizeAA]*sizeof(long));
			A[currentSizeAA][currentSizeA[currentSizeAA]-1] = tempC;
		}
		}
		while ((tempC = strtol(pEnd, &pEnd, 10)) != 0);	
		A = (long **) realloc(A,++currentSizeAA*sizeof(long *));
		currentSizeA = realloc(currentSizeA, currentSizeAA*sizeof(int));
	}
MatrixB:
printf("END OF MATRIX A");

	//Matrix B
	int currentSizeB = 0;
	memset(&tempString[0], 0, sizeof(tempString));
	while (scanf(" %[^\n]",tempString) == 1) {
		if ('\n' == tempString[0]) break;	
		char *pEnd = &tempString[0];
		long tempC = strtol(pEnd, &pEnd, 10);
		do {
printf("%ld\n", tempC);
			B = (long *) realloc(B, ++currentSizeB*sizeof(long));
			B[currentSizeB-1] = tempC;
	  	}
		while ((tempC = strtol(pEnd, &pEnd, 10)) != 0);	
	}
printf("END OF MATRIX B");
	

	//Output
	for (int i = 0; i <= currentSizeAA-1; i++) 
		for (int k = 0; k < currentSizeA[i]; k++) 
			printf("%ld\t", A[i][k]);

	free(A);
//	free(B);
	return 0;
}

