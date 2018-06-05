#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main( int argc,char * argv[]) {
	 int j;
	 for(j=0; j < atoi(argv[2]); j++) {
	 	printf("Potomny %s krok %d\n", argv[1], j);
	 	sleep(1);
	 }
 	exit(atoi(argv[1]));
} 
