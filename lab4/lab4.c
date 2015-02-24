/** Victor Wu
  CS350 - Lab 4
  Program designed to take up larger memory in space, but more efficient so that there will not be as many FILE I/O operations - making it faster, overall */

#define INITIAL_CAPACITY 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student {
	char name[32];
	char phone[16];
	char address[128];
	int id;
} student_t;

FILE* fp;
int studentSize = INITIAL_CAPACITY, currentSize = 0, actualSize = 0;

int createStudent(student_t *myStudent, int id, char name[32], char phone[16], char address[128]) {
	if (currentSize >= studentSize) {
		studentSize *= 2;
		return -1;	
	}
	student_t *tempStudent = (student_t *) &(myStudent[id]);
	snprintf((*tempStudent).name, sizeof((*tempStudent).name), "%s", name);
	snprintf((*tempStudent).phone, sizeof((*tempStudent).phone), "%s", phone);
	snprintf((*tempStudent).address, sizeof((*tempStudent).name), "%s", address);
	(*tempStudent).id = id;
	int i = 0;
	for (; i < currentSize-1; i++)
	{
		if (strcmp((myStudent[i]).name, name) == 0 && strcmp((myStudent[i]).phone,  phone) == 0 && strcmp((myStudent[i]).address, address) == 0)
			return i+1;
	}
	return 0;
}


int main (void) {
	fp = fopen("students", "a+");
	//Read the keyed file
	if (fp == NULL) {
		fprintf(stderr, "Can't open the input file \"students\". Exiting.\n");
	}
	student_t *myStudent = calloc(INITIAL_CAPACITY,sizeof(student_t));

	//Loading
	char myName[32];
	char myPhone[16];
	char myAddress[128];
	int myID = 0;
	while (fscanf(fp, " %32[^,\n], %16[^,\n], %128[^,\n], %d", myName, myPhone, myAddress, &myID) == 4) {
		int x = 0;
		x = createStudent(myStudent, currentSize++, myName, myPhone, myAddress);
		if (x == -1) myStudent = (student_t *) realloc(myStudent, sizeof(student_t)*studentSize);
	}
	if ((actualSize = myID + 1) != currentSize)	printf("Error: Read only %d records, but there seems to be a record of %d", currentSize, actualSize); 

	//Finished loading

	char bool = 'N';
	while (bool != 'y' && bool != 'Y') {
		printf("\nName >>\t");
		scanf(" %s", myName);
		printf("Phone >>\t");
		scanf(" %s", myPhone);
		printf("Address >>\t");
		scanf(" %s", myAddress);
		int x = createStudent(myStudent, currentSize++, myName, myPhone, myAddress);
		if (x > 0) {
			printf("Student duplicate found. Is this a new entry? (Y/N)\t");
			scanf(" %c", &bool);
			if (bool != 'y' && bool != 'Y') {
				currentSize--;
			}
		}
		else if (x == -1) myStudent = (student_t *) realloc(myStudent, sizeof(student_t)*studentSize);
		printf("Quit(y/n)?\t");
		getchar(); //Gets rid of newline	
		bool = getchar();	
	}

	for (int i = actualSize; i < currentSize; i++) {
		fprintf(fp, "%s,%s,%s,%d\n", (myStudent[i]).name, (myStudent[i]).phone, (myStudent[i]).address, (myStudent[i]).id);
	}	
	free(myStudent);
	return 0;
}

