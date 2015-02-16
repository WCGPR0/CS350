#include <stdio.h>
#include <assert.h>
FILE *fpIn, *fpOut;

void charMode() {
int tempChar;
char *myLine = malloc (1000); //Change the 1000 depending on specification or estimation of spec.
size_t counter = 0;

while (tempChar = getc(fpIn) != EOF) {
	if (feof(fpIn)) break;
	//fputc(tempChar, fpOut);	
	myLine[counter++] = (char) tempChar;
	if (tempChar == '\n' && tempChar != EOF) { //Resets line
		printf("%s", myLine);
		memset(myLine, 0, 1000);
		counter = 0;
	}
}
}

void lineMode(){
ssize_t readSize;
size_t len = 0;
char *line = NULL;
while (readSize = getline(&line, &len, fpIn) != -1) {
	fprintf(fpOut, "%s", line);
}
}

int main()
{
int option = 3;
while (option != 1 && option != 0) {
printf("Please choose an option:\n0 for character-by-character mode\n1. for line-by-line mode\n");
scanf("%d", &option);
}


fpIn = fopen("./test.txt", "r");
fpOut = fopen("./test.out", "w");
	assert(fpIn != NULL | fpOut != NULL);

if (option == 1) lineMode();
else if (option == 0) charMode();
fclose(fpIn);
fclose(fpOut); 
return 0;
}
