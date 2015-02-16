#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "time_functions.h"
using namespace std;

void simple (void);
void compound (void);

#define asize 100000
#define outer 20000

double x[asize], y [asize], z[asize];

int main(void)
{ int i,junk;string timefinal;	
	for (i=0;i<asize;i++)
	   y[i]=i; z[i]=i;
	printf("test starting\n");
  /* *** clock() & ftime() only give wall-clock time on Windows */
  /* *** difftime() gives a long integer value (without millisecs) */

	/* --------------- CASE 1 ------------------------- */
	start_timing();
	simple();
	stop_timing();

	printf("the time from 'ftime' for simple 1 is %10.3f seconds \n ",get_wall_clock_diff());
	printf("the time from 'getCPUtime' for simple 1 is %10.3f seconds\n",get_CPU_time_diff());  // good for seconds of wall clock time	
	printf("\n");
	printf("end simple 1\n");
	printf("\n\n\n");
		
	/* --------------- CASE "compound 1" ------------------------- */
	start_timing();
	compound();
	stop_timing();

	printf("the time from 'ftime' for compound 1 is %10.3f seconds\n ",get_wall_clock_diff());	
	printf("the time from 'getCPUtime' for compound 1 is %10.3f seconds\n",get_CPU_time_diff());  // good for seconds of wall clock time	
	printf("\n");
	printf("end compound 1\n");
	printf("\n\n\n");

	/* ------------------------  sleep case *****************/
	start_timing();
#ifdef WIN32	
	Sleep(2); // Windows does it in whole secs (and a capital "S")
#else
	sleep(2000);  // linux does it in mils and a lowercase "s"
#endif
	stop_timing();
	
	printf("the time from 'ftime' for sleepcase is %10.3f seconds\n ",get_wall_clock_diff());
	printf("the time from 'CPUtime' for sleepcase is %10.3f seconds\n",get_CPU_time_diff());  // good for seconds of wall clock time	
	/* --------------- ------ ------------------------- */  
	printf("testing complete. Enter any number to close the window.\n");
	junk=getchar();  // don't care if it's an eof or not. Any interrupt is OK
	return 0;
}
void simple (void)
{ int i,j;
	for (i=0;i<asize;i++)
		x[i]=y[i]/4+z[i]/4;
}

void compound(void)
{ int i,j;
	for (j=0;j<outer;j++)
	for (i=0;i<asize;i++)
		x[i]=y[i]/4+z[i]/4;
}



