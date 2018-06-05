#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define PROCES_NUM 10


int main(int argc, char *argv[]) {

    int child_num = argc - 1;

    for(int i = 0; i < child_num; i++) {
        pid_t pid = fork();

        if(pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        if(pid == 0) {
            char* args[] = {"./child",};
            char child_steps[32];
            sprintf(child_steps, "%d", i);
            args[1] = child_steps;
            execl(args[0], args);
        }

    }

    // Parent Process
    printf("Proces macierzysty, pid: %d", getpid());

    for(int i = 0; i < child_num; i++) {
        wait(NULL);
    }

    return 0;
}
