#include <stdio.h>
#include <signal.h>


static volatile int loop = 1;

void myHandler(int someSig) {
	printf("\nOH NO!!!\n");
	fflush(stdout);	
}

int main (int argc, char *argv[]) {
struct sigaction myAct;
myAct.sa_handler = myHandler;
sigaction(SIGINT, &myAct, NULL);
sigaction(SIGTSTP, &myAct, NULL);
	//signal(SIGINT, myHandler);
while (loop) {
//	pause();
}

return 0;
}
