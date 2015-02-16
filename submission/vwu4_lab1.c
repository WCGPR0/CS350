#include "stdio.h"
#include "time_functions.h" //This is a header file from Professor Foreman's class

int main(void) {

printf("Please provide the name of an input file \n");
char inputFileName[256]; //Filenames should be limited to 255 characters
scanf("%s", inputFileName);

printf("Please provide the name of an output file \n");
char outputFileName[256]; //Filenames should be limited to 255 characters
scanf("%s", outputFileName);

start_timing();

FILE *fpIn, *fpOut;
fpIn = fopen(inputFileName, "r");
fpOut = fopen(outputFileName, "w");

unsigned char buffer[256]; // The size of buffer is One Byte

while ( !feof (fpIn)) {
	fread(&buffer, sizeof(unsigned char), 1, fpIn);
//		if (ferror (fpIn) ) {
//			printf("Error reading file");
//			break;
//		}
	fwrite(&buffer, sizeof(buffer[0]), 1, fpOut);
//	fwrite(&buffer, 1, 1, fpOut);
}
printf("%d",sizeof(buffer[0]));

fclose(fpOut);


stop_timing();
//printf("%10.3f", get_wall_clock_diff());
printf("CPU Time: %10.3f\n", get_CPU_time_diff());

return 0;
}
