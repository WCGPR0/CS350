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
	long *A = (long *) malloc(sizeof(long)), *B = (long *) malloc(sizeof(long));

	//Matrix A
	int currentSizeA = 0;
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
			A = (long *) realloc(A, ++currentSizeA*sizeof(long));
			A[currentSizeA-1] = tempC;
		}
		}
		while ((tempC = strtol(pEnd, &pEnd, 10)) != 0);	

	}
MatrixB:
printf("END OF MATRIX A");

	//Matrix B
	int currentSizeB = 0;
	memset(&tempString[0], 0, sizeof(tempString));
	while (1) {
		scanf(" %[^\n]", tempString);
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

	
	//Output
	for (int i = 0; i < currentSizeA; i++) {
		printf("%ld\t", A[i]);
	}

	free(A);
	free(B);
	return 0;
}

