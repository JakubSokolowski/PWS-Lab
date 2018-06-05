#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// With L: comma separated arguments
// With V: array of strings
// With P: include normal search path for executable

int main(int argc, char *argv[]) {
	printf("execDemo: My pid is %d.\n", getpid());	
	char *args[] = {"./helloExec", "Hello", "World", NULL};
	execvp("./helloExec", args);
	return 0;
}
