#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int main (void) {
	int N;
	char str[100];
	scanf("%d", N);
	gets(str);
	N--;

	pid_t childPID = fork();
	int fd[2];
	pipe(fd);
	if (childPID >= 0) {
		if (childPID == 0) { //Child process
			if (N > 0) { 
				N--;	
				childPID = fork();
			}
			else if (N == 0) {
				close(fd[1]);
				read(fd[0], str, sizeof(str));
			}
		}	
		else { //Parent process	
			close(fd[0]);
			write(fd[1], str, strlen(str)+1);
			wait(NULL);
			exit(0);
		} 

	}
	else {
		printf("There was a fork error");
		exit(1);
		}
}
