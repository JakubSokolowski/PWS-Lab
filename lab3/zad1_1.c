#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
	int pid,status, num_child;
	num_child = argc-1;
	for(int i = 0; i <num_child; i++){
		if((pid = fork()) == 0) {
		 /* Proces potomny ---*/
			for(int j = 0; j < i; j++) {
				printf(" Potomny %d krok: %d\n",i,j);
				sleep(1);
			}
		exit(i);
		}
	}
	
	
	
	
	/* Proces macierzysty */
	printf("Macierzysty = %d \n",getpid());
	for(int i = 0; i < num_child; i++)
	{
		pid = wait(&status);
		printf("Proces zakonczony statusem %d\n", WEXITSTATUS(status));
	}
	
	printf("Proces %d  zkonczony: %d\n",pid,status);
}

