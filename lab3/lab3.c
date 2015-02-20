/** Victor Wu
  CS350
  Program designed to take up larger memory in space, but more efficient so that there will not be as many FILE I/O operations - making it faster, overall */

#define INITIAL_CAPACITY 100

#include <stdio.h>
#include <stdlib.h>

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
		myStudent = (student_t *) realloc(myStudent, sizeof(student_t)*studentSize);
	}
	myStudent = (student_t *) (myStudent+(id*sizeof(student_t)));
	snprintf((*myStudent).name, sizeof((*myStudent).name), "%s", name);
	snprintf((*myStudent).phone, sizeof((*myStudent).phone), "%s", phone);
	snprintf((*myStudent).address, sizeof((*myStudent).name), "%s", address);
	(*myStudent).id = id;
	//*(myStudent+currentSize*sizeof(student_t)) = newStudent;
	//free(newStudent);
	int i = 0;
	for (; i < currentSize; i++)
	{
		int x = i*sizeof(student_t);
		if ((*(myStudent+x)).name == name || (*(myStudent+x)).phone == phone || (*(myStudent+x)).address == address) break;
	}
	if (i >= currentSize) return 0;
	else return i;
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
	int myID;
	while (fscanf(fp, "%s %s %s %d", myName, myPhone, myAddress, &myID) == 3) {
		createStudent(myStudent, myID, myName, myPhone, myAddress);
	}
	actualSize = currentSize = myID;

	//Finished loading

	char bool = 'Y';
	while (bool != 'y' && bool != 'Y') {
		printf("\nName >>\t");
		scanf(" %s", myName);
		printf("Phone >>\t");
		scanf(" %s", myPhone);
		printf("Address >>\t");
		scanf(" %s", myAddress);
		int x = createStudent(myStudent, currentSize++, myName, myPhone, myAddress);
		if (x != 0) {
			printf("Student duplicate found. Is this a new entry? (Y/N)\t");
			scanf(" %c", &bool);
			if (bool != 'y' && bool != 'Y') {
				currentSize--;
			}
		}
		printf("Record Created with id: %d", currentSize);
		printf("Quit(y/n)?\t");
		getchar(); //Gets rid of newline	
		bool = getchar();	
	}

	for (int i = 0; i < currentSize; i++) { //Replace 0 with actualSize
		fprintf(fp, "%s,%s,%s,%d\n", (*(myStudent+(i*sizeof(student_t)))).name, (*(myStudent+(i*sizeof(student_t)))).phone, (*(myStudent+(i*sizeof(student_t)))).address, (*(myStudent+(i*sizeof(student_t)))).id);
	}	

	return 0;
}

