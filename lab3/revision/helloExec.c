#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	
	printf("helloExec.c:\n");
	printf("\tMy Pid %d.\n", getpid());

	printf("\tMy arguments: ");
	for(int i = 0; i< argc; i++) {
		printf("%s, ", argv[i]);
	}
	printf("\n");
	return 0;
}

