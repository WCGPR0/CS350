#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main (void) {
	int N;
	char str[100];
	scanf("%d", &N);
	scanf("%s", str);
	//fgets(str, sizeof(str), stdin);
	N--;

	pid_t childPID = fork();
	int fd[2];
	pipe(fd);
	if (childPID >= 0) {
		if (childPID == 0) { //Child process
			if (N > 0) { 
			close(fd[1]);
	CHILD:			N--;	
				childPID = fork();
				if (N > 0) goto CHILD;
			}
			 else if (N == 0) {	
				read(fd[0], str, sizeof(str));	
				printf("%s", str);
				exit(0);
			}
		}	
		else { //Parent process	
			close(fd[0]);
			write(fd[1], str, strlen(str));
			wait(NULL);
			exit(0);
		} 
	}
	else {
		printf("There was a fork error");
		exit(1);
		}
	return 0;
}
