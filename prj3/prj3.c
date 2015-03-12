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
	int *A = malloc(1), *B = malloc(1);

	//Matrix A
	int loop = 1, currentSizeA = 0;
	char tempString[256];
	while (scanf("%s[^\n]", tempString)) {
		char tempChar;
		while (sscanf(tempString, " %c", &tempChar)) {
			A = (int *) realloc(A, ++currentSizeA);	
			int counter = 0;
			while (tempChar == '*') {
				if (++counter > 8) error(1);
				break;break;
			}
			A[currentSizeA] = tempChar;
		}
	}

	
	//Output
	for (int i = 0; i < currentSizeA; i++) {
		printf("%i", A[i]);
	}

	return 0;
}

