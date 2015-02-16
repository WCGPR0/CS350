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
} student_t;

static int id = 0;
FILE* fp;
int studentSize = INITIAL_CAPACITY, currentSize = 0;

int createStudent(student_t **myStudent, int currentSize, char name[32], char phone[16], char address[128]) {
if (currentSize >= studentSize) {
	studentSize *= 2;
	myStudent = realloc(*myStudent, sizeof(student_t)*studentSize);
	}
student_t* newStudent = malloc(sizeof(student_t));
snprintf((*newStudent).name, sizeof((*newStudent).name), "%s", name);
snprintf((*newStudent).phone, sizeof((*newStudent).phone), "%s", phone);
snprintf((*newStudent).address, sizeof((*newStudent).name), "%s", address);
*(myStudent+currentSize*sizeof(student_t)) = newStudent;
free(newStudent);
int i = 0;
for (; i < currentSize; i++)
{
int x = i*sizeof(student_t);
if ((*(*myStudent+x)).name == name || (*(*myStudent+x)).phone == phone || (*(*myStudent+x)).address == address) break;
}
if (i >= currentSize) return 0;
else
return i;
}


int main (int argc, char *argv[]) {
fp = fopen("students", "a+");
//Read the keyed file
if (fp == NULL) {
	fprintf(stderr, "Can't open the input file \"students\". Exiting.\n");
}
student_t *myStudent = malloc(INITIAL_CAPACITY*sizeof(student_t));

//Loading
char myName[32];
char myPhone[16];
char myAddress[128];
while (fscanf(fp, "%s %s %s", myName, myPhone, myAddress) == 3) {
	createStudent(&myStudent, currentSize, myName, myPhone, myAddress);
}

//Finished loading

while (1) {
char bool;
	printf("Please enter the following information for a student:\n");	
	printf("Name:\t");
	scanf("%s", myName);
	printf("Phone:\t");
	scanf("%s", myPhone);
	printf("Address:\t");
	scanf("%s", myAddress);
	int x = createStudent(&myStudent, currentSize++, myName, myPhone, myAddress);
	if (x != 0) {
		printf("Student duplicate found. Is this a new entry? (Y/N)\t");
		scanf("%c", bool);
		if (bool != 'y' && bool != 'Y') {
			currentSize--;
		}
	}
	printf("Quit? (Y/N)\t");
	getchar(); //Gets rid of newline	
	bool = getchar();
	if (bool == 'y' || bool == 'Y') break;
}

for (int i = 0; i < currentSize; i++) {
fprintf(fp, "%s %s %s", myStudent[i].name, myStudent[i].phone, myStudent[i].address);
//printf("hey there");
//fprintf(fp, "%s", "hey there");
}	

return 0;
}

