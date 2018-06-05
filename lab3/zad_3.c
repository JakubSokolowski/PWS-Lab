#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char * argv[]){
	int pid,j,status;
	
	//char kroki[20];
	int num_child = argc - 1;
	
	char buf[80];
	for(int i = 0 ; i < num_child; i++) {
		if((pid = fork()) == 0) { 
			//sprintf(kroki,"%d",i);
			//execl("./pot2","pot2",kroki,argv[j+2] ,NULL);
			//perror("Blad funkcji exec");
			sprintf(buf,"./pot2 %d %s", i+1, argv[i+2]);
			system(buf);
			exit(i+1); 
 		}
 		else
 		{
 			for(j=1;j < atoi(argv[1]); j++) {
				printf("Macierzysty - krok %d \n",j);
				sleep(1); 
			}			
 		}
 		 
		
		
	}
	
	
	for (int i = 0; i < num_child; i++) {
        pid = wait(&status);
        status = WEXITSTATUS(status);
        printf("Proces %d exited with status: %d\n",pid, status);
    }	
}
