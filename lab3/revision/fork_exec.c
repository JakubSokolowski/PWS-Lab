#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 2
// Use WEXITSATUS macro to get int value from status
int main(int argc, char *argv[]) {
	for(int i = 0; i < NUM_CHILDREN; i++) {
		pid_t pid = fork();
       	
		if(pid < 0) {
			perror("fork failed");
			exit(1);
		}
		if(pid == 0) {	
		// Child execs
			char* args[] = {"./bogosort", "1", "2","3",NULL};
			execv(args[0], args);
		}
	}

	// Parent
	
	printf("Parent is waitng on child process.\n");
	for(int i = 0; i < NUM_CHILDREN; i++) {
		wait(NULL);	
	}
	
	return 0;	
}
