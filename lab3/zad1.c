#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

    int child_num = argc - 1;
    int pid, status;
	printf("Child num: %d\n", child_num);
    for (int i = 0; i < child_num; i++) {
        if ((pid = fork()) == 0) {
			for(int j = 0; j < 5; j++)
            	printf("Proces potomy %d krok %d\n", i, j);
            sleep(1);
        }
		
        exit(i+1);
    }

    // Parent Process
    printf("Proces macierzysty, pid: %d\n", getpid());

    for (int i = 0; i < child_num; i++) {
        wait(&status);
        status = WEXITSTATUS(status);
        printf("Proces exited with status: %d\n", status);
    }

    return 0;
}
